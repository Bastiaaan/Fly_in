//
// Created by brogaar on 30-6-26.
//

#include <any>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <regex>
#include "fly_in.hpp"

using _json = nlohmann::json;

bool isNumeric(std::string const &s);

System::System()
{
    this->nb_drones = 0;
    this->level = 0;
    this->score = 0;
    this->difficulty = "TBD";
    this->turn = 0;
    this->_map = Map();
    this->logs = std::map<int, std::vector<Log*>>();
}

System::~System()
{
    cout << endl << "==== shutting down ====" << endl;
	if (this->logs.empty())
		return;
	for (auto found : this->logs)
	{
		std::vector<Log*> logs = found.second;
		for (Log * log : logs)
		{
			delete log;
			log = nullptr; // to prevent dangling pointers after 'free'.
		}
	}
}

void System::verboseFree(ExecuteState const &ex)
{
    if (!ex.success)
    {
        if (ex.line != std::nullopt)
            cout << "ERROR " << std::any_cast<std::string>(ex.why) << endl << "At line " << std::any_cast<int>(ex.line) << endl;
        else
            cout << "ERROR: " << std::any_cast<std::string>(ex.why) << endl;
    }
    else
        cout << "No errors found" << endl;
}

void System::verboseLog(int const n)
{
	std::vector<Log*> logsPerTurn = this->logs[n];
	if (logsPerTurn.empty())
		return;
	for (Log const *log : logsPerTurn)
		std::cout << log->_output << " ";
	std::cout << std::endl;
}

ExecuteState System::mapsBasePath()
{
    try
    {
        std::string const currentPath = filesystem::current_path().string();
        vector<std::string> const parts = split(currentPath, '/');
        unsigned int fly_in = 0;
        while (parts[fly_in] != "fly_in")
            fly_in++;
        fly_in++;
        std::string const _core = join(parts, '/', fly_in);
        std::string const &result = _core + "/parser/result/";
        return ExecuteState::Ok(result);
    }
    catch (exception &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}

ExecuteState System::registerTurn()
{
    this->turn++;
    return ExecuteState::Ok(1);
}

const Hub &System::findByCoordinates(int const x, const int y)
{
    auto hub = [this](int x, int y) -> Hub*
    {
        for (auto& h : this->_map.hubs)
        {
            if (h->position_x == x && h->position_y == y)
                return h;
        }
        return nullptr;
    };
    return *hub(x, y);
}

ExecuteState System::jsonMap()
{
    static const std::regex pattern(R"(^(map|fail)\.json$)");

    auto const mapResult = mapsBasePath();
    if (!mapResult.success)
    {
        verboseFree(mapResult);
        return ExecuteState::Fail("noppus");
    }

    std::error_code ec;
    auto it = std::filesystem::directory_iterator(
        std::any_cast<std::string>(mapResult.result), ec);
    if (ec)
    {
        return ExecuteState::Fail("directory not readable");
    }

    std::filesystem::directory_iterator end;
    if (it == end)
    {
        return ExecuteState::Fail("no file found");
    }

    std::string fileName = it->path().filename().string();

    ++it;
    if (it != end)
    {
        return ExecuteState::Fail("more than one file found");
    }
    if (!std::regex_match(fileName, pattern))
    {
        return ExecuteState::Fail("file does not match expected pattern");
    }

    return ExecuteState::Ok(fileName);
}

ExecuteState System::extractMap(std::string const &file)
{
    _json json;
    try
    {
        ExecuteState const pathRes = mapsBasePath();
        if (!pathRes.success)
            throw ParseException("path base crash");
        std::string const base = std::any_cast<std::string>(pathRes.result);
        std::ifstream data(base + file);
        if (!data.is_open())
            throw ParseException("Could not fetch " + (base + file));
        data >> json;
        this->nb_drones = json["nb_drones"];
        this->_map.name = json["name"];
        for (auto _hub : json["hubs"])
        {
            Hub *hub = new Hub();
            hub->setName(static_cast<std::string>(_hub["name"]));
            hub->setX(static_cast<int>(_hub["x"]));
            hub->setY(static_cast<int>(_hub["y"]));
            hub->setColor(static_cast<std::string>(_hub["color"]));
            hub->setZone(static_cast<std::string>(_hub["zone"]));
            if (_hub["position"] != "midway")
            {
                _hub["position"] != "start"
                    ? hub->setStartOrEnd("end")
                    : hub->setStartOrEnd("start");
                hub->setMaxDrones(this->nb_drones);
            }
            else
                hub->setMaxDrones(static_cast<int>(_hub["max_drones"]));
            this->_map.addHub(hub);
        }
        for (auto _connection : json["connections"])
        {
            Connection *connection = new Connection();
            Hub *hub1 = this->_map.getHub(static_cast<std::string>(_connection["hub_1"]));
            if (hub1 == nullptr)
                throw ParseException("first hub not found: " + static_cast<std::string>(_connection["hub_1"]));
            connection->setHub1(hub1);
            Hub *hub2 = this->_map.getHub(static_cast<std::string>(_connection["hub_2"]));
            if (hub2 == nullptr)
                throw ParseException("second hub not found: " + static_cast<std::string>(_connection["hub_2"]));
            connection->setHub2(hub2);
            connection->setMlc(static_cast<int>(_connection["max_link_capacity"]));
            this->_map.addConnection(connection);
            auto established = connection->validate_connection(this->_map);
            if (!established.success)
                throw ParseException(established.why.value());
        }
        for (auto conn : this->_map.connections)
        {
            delete conn;
            conn = nullptr;
        }
        return ExecuteState::Ok("success");
    }
    catch(ParseException &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}

ExecuteState System::initDrones()
{
    try
    {
        Hub *start = this->_map.hubs.front();
        if (start == nullptr)
            throw logic_error("Start hub was not found");
        for (unsigned int id = 0; id < this->nb_drones; id++)
        {
            std::cout << "Installing drone #" << id + 1 << std::endl;
            auto *drone = new Drone(id + 1);
            start->drones.push_back(drone);
        }
        return ExecuteState::Ok(TextFormat("%d drones installed", this->nb_drones));
    }
    catch (std::exception &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}
