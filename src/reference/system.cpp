//
// Created by brogaar on 30-6-26.
//

#include <any>
#include <optional>
#include <iostream>
#include <fstream>
#include "fly_in.hpp"
#include "factory.tpp"

System::System()
{
    this->nb_drones = 0;
    this->level = 0;
    this->score = 0;
    this->difficulty = "TBD";
    this->turn = 0;
    this->_map = Map();
    this->validator = new Validator();
    this->logs = std::map<int, std::vector<Log*>>();
}

System::~System()
{
    cout << endl << "==== shutting down ====" << endl;
    delete this->validator;
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
        std::string const &result = _core + "/maps/";
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

ExecuteState System::Load(std::string const &level, std::string const &difficulty)
{
    auto mapResult = mapsBasePath();
    if (mapResult.success)
    {
        unsigned int line = 0;
        auto result = mapResult.result;
        std::string const &base = std::any_cast<std::string>(result);
        std::string const target = base + difficulty + '/' + level;
        std::ifstream _mapped(target);
        if (!_mapped.is_open())
            return ExecuteState::Fail("Could not open map '" + target + "'", 0);
        vector<Connection> collectedConnections;
        try
        {
            std::string buffer;
            while (std::getline(_mapped, buffer))
            {
                line++;
                if (!buffer.empty())
                {
                    if (buffer[0] != '#')
                    {
                        ExecuteState res = this->validator->saveRec(buffer, line);
                        if (!res.success)
                            throw std::logic_error(std::any_cast<std::string>(res.why));
                        vector<string> keyRow = split(buffer, ':');
                        if (keyRow.size() != 2) {
                            _mapped.close();
                            return ExecuteState::Fail("Each row should only contain one double column", line);
                        }
                        std::string const &key = keyRow[0];
                        std::string const &row = keyRow[1];
                        if (key == "start_hub" || key == "end_hub" || key == "hub")
                        {
                            std::vector<std::string> values = split(row, ' ');
                            unsigned int opened_meta = 0;
                            int pos = 0;
                            values.erase(values.begin());
                            std::map<int, Argument> args = Factory<Hub>::ready_args();
                            for (const auto &rec : values)
                            {
                                if (rec.find('[') != std::string::npos)
                                    opened_meta++;

                                if (opened_meta > 0)
                                {
                                    std::vector<std::string> meta_pairs = split(rec, '=');
                                    if (meta_pairs.size() != 2)
                                        throw std::logic_error("unusual assignments found");
                                    std::string metaName = meta_pairs[0];
                                    std::string metaValue = meta_pairs[1];
                                    if (metaName.find('[') != std::string::npos && metaName[0] == '[')
                                        metaName.erase(0, 1);
                                    if (metaValue.find(']') != std::string::npos && metaValue[metaValue.size() - 1] == ']')
                                        metaValue.erase(metaValue.size() - 1, 1);
                                    int index = Factory<Hub>::resolveKey(metaName, args);
                                    args[index].value = metaValue;
                                    if (rec.find(']') != std::string::npos)
                                        opened_meta--;
                                }
                                else
                                {
                                    args[pos].value = rec;
                                    pos++;
                                }
                            }
                            if (opened_meta != 0)
                                throw std::logic_error("Incorrect meta enclosure found");
                            auto _hub = new Hub(Factory<Hub>::create(args));
                            if (_hub->position_x < 0)
                                throw ParseException("first number cannot be negative");
                            if (key == "start_hub")
							{
                                _hub->setStartOrEnd("start");
								if (this->nb_drones > 0)
									_hub->max_drones = this->nb_drones;
							}
                            else if (key == "end_hub")
							{
                                _hub->setStartOrEnd("end");
								if (this->nb_drones > 0)
									_hub->max_drones = this->nb_drones;
							}
                            this->_map.addHub(_hub);

                        }
                        else if(key == "connection")
                        {
                            auto args = Factory<Connection>::ready_args();
                            std::vector<std::string> values = split(row, ' ');
                            values.erase(values.begin()); // necessary to cut out empty records.
                            if (values.empty() || values.size() > 2)
                                throw logic_error("Connection definition must consist out of <hub_name>-<hub_name> [...]");
                            std::vector<std::string> hubs = split(values[0], '-');
                            if (hubs.size() != 2)
                                throw logic_error("Invalid connection definition detected");
                            if (this->_map.getHub(hubs[0]) == nullptr)
                                throw range_error("First hub is not found");
                            if (this->_map.getHub(hubs[1]) == nullptr)
                                throw range_error("Second hub is not found");
                            int hub1 = Factory<Connection>::resolveKey("hub1", args);
                            int hub2 = Factory<Connection>::resolveKey("hub2", args);
                            args[hub1].value = this->_map.getHub(hubs[0]);
                            args[hub2].value = this->_map.getHub(hubs[1]);
                            if (values.size() == 2)
                            {
                                if (values[1].find('[') != std::string::npos &&
                                    values[1].find(']') != std::string::npos)
                                {
                                    std::vector<std::string> metaKeyValue = split(values[1], '=');
                                    if (metaKeyValue.size() != 2)
                                        throw std::logic_error("Connection row only expects one optional record");
                                    std::string _key = metaKeyValue[0].erase(0, 1);
                                    std::string value = metaKeyValue[1].erase(metaKeyValue[1].length() - 1, 1);
                                    int insert = Factory<Connection>::resolveKey(_key, args);
                                    args[insert].value = value;
                                }
                                else if (values[1].find('[') == std::string::npos ||
                                         values[1].find(']') == std::string::npos)
                                    throw std::logic_error("Custom brackets enclosed incorrectly");
                            }
                            auto connection = new Connection(Factory<Connection>::create(args));
                            this->_map.addConnection(connection);
                            ExecuteState connect_result = connection->validate_connection(this->_map);
                            if (!connect_result.success)
                                throw ParseException(connect_result.why.value());
                        }
                        else if(key == "nb_drones")
                        {
                            int value = stoi(row);
                            if (!value)
                                throw ParseException("nb_drones must contain a numeric value.");
                            if (value <= 0)
                                throw ParseException("nb_drones can never be negative or zero.");
                            this->nb_drones = value;
                        }
                        else
                            throw ParseException("Unknown config key found.");
                    }
                }
            }
            _mapped.close();
            for (auto &con : this->_map.connections)
            {
                delete con;
                con = nullptr;
            }
            return ExecuteState::Ok("Ok");
        }
        catch (ParseException &pex)
        {
            _mapped.close();
            return ExecuteState::Fail(pex.what(), line);
        }
        catch (std::exception &ex)
        {
            _mapped.close();
            return ExecuteState::Fail(ex.what(), line);
        }
    }
    return mapResult;
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

std::vector<tuple<int, std::string, std::string>>
	System::get_options(std::string const &difficulty)
{
    cout << "choosing out of options..." << endl;
    std::vector<tuple<int, std::string, std::string>> options;
	// <level, display_name, file>
    int opt = 0;
    regex pattern(R"(^\d{1,2}_[a-z]+(?:_[a-z]+)*\.txt$)");
    auto const mapResult = mapsBasePath();
    if (!mapResult.success) {
        verboseFree(mapResult);
        return options;
    }
    string const _core = std::any_cast<std::string>(mapResult.result) + difficulty + '/';
    for (const auto& fileSrc : filesystem::directory_iterator(_core))
    {
        opt++;
        std::vector<string> broken = split(fileSrc.path().string(), '/');
        std::string fileName = broken[broken.size() - 1];
        std::vector fileParts = split(split(fileName, '.')[0], '_');
        fileParts.erase(fileParts.begin());
        options.push_back({opt, join(fileParts, ' '), fileName});
    }
    return options;
}