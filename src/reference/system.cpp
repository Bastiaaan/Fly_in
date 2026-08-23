//
// Created by brogaar on 30-6-26.
//

#include <any>
#include <optional>
#include <iostream>
#include <fstream>
#include "fly_in.hpp"
#include "factory.tpp"

bool isNumeric(std::string const &s);

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
        int start = 0, end = 0;
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
                    ExecuteState res = this->validator->saveRec(buffer, line);
                    if (!res.success)
                    {
                        _mapped.close();
                        throw std::logic_error(any_cast<std::string>(res.why));
                    }
                }
            }
            _mapped.close();
            for (auto record : this->validator->mapRows)
            {
                std::string rule = record.second;
                if (!rule.empty())
                {
                    if (rule[0] != '#')
                    {
                        int number = static_cast<int>(record.first);
                        std::string forEx = std::to_string(number);
                        vector<string> keyRow = split(rule, ':');
                        if (keyRow.size() != 2) {
                            return ExecuteState::Fail("Each row should only contain one double column", number);
                        }
                        std::string const &key = keyRow[0];
                        std::string const &row = keyRow[1];
                        if (key == "start_hub" || key == "end_hub" || key == "hub")
                        {
                            if (key == "start_hub")
							{
                                start++;
                                if (start > 1)
                                {
                                    this->validator->saveError(
                                        FailureCause::MultiStartHubs,
                                        "that's not allowed");
                                }
							}
                            else if (key == "end_hub")
							{
                                end++;
                                if (end > 1)
                                {
                                    this->validator->saveError(
                                        FailureCause::MultiEndHubs,
                                        "that's not allowed");
                                }
							}
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
                                    {
                                        this->validator->saveError(
                                            FailureCause::BadRuleFormation,
                                            "for hub extraction", number);
                                        continue;
                                    }
                                            
                                    std::string metaName = meta_pairs[0];
                                    std::string metaValue = meta_pairs[1];
                                    if (metaName.find('[') != std::string::npos && metaName[0] == '[')
                                        metaName.erase(0, 1);
                                    if (metaValue.find(']') != std::string::npos && metaValue[metaValue.size() - 1] == ']')
                                        metaValue.erase(metaValue.size() - 1, 1);
                                    int index = Factory<Hub>::resolveKey(metaName, args);
                                    if (index != -1)
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
                            {
                                std::string msg = opened_meta > 0
                                    ? "missing enclosure ']'"
                                    : "missing opening '['";

                                this->validator->saveError(
                                    FailureCause::IncorrectMetaFormation,
                                    msg, number);
                                continue;
                            }
                            if (Factory<Hub>::getVal(args, "name").empty()
                             || Factory<Hub>::getVal(args, "x").empty()
                             || Factory<Hub>::getVal(args, "y").empty())
                            {
                                if (Factory<Hub>::getVal(args, "name").empty())
                                    this->validator->saveError(
                                        FailureCause::IncorrectNameValue,
                                        "name is required", number);

                                if (Factory<Hub>::getVal(args, "x").empty())
                                    this->validator->saveError(
                                        FailureCause::IncorrectLocValue,
                                        "first number is required", number);

                                if (Factory<Hub>::getVal(args, "y").empty())
                                    this->validator->saveError(
                                        FailureCause::IncorrectLocValue,
                                        "second number is required", number);
                                args.clear();
                                continue;
                            }
                            else if (!isNumeric(Factory<Hub>::getVal(args, "x"))
                                  || !isNumeric(Factory<Hub>::getVal(args, "x")))
                            {
                                if (!isNumeric(Factory<Hub>::getVal(args, "x")))
                                {
                                    this->validator->saveError(
                                        FailureCause::IncorrectLocValue,
                                        "first number needs to be a numberic value", number);
                                    args[Factory<Hub>::resolveKey("x", args)].value = forEx;
                                    // assigning forEx to prevent crashing by an std::bad_cast<>
                                }
                                if (!isNumeric(Factory<Hub>::getVal(args, "y")))
                                {
                                    this->validator->saveError(
                                        FailureCause::IncorrectLocValue,
                                        "second number needs to be a numberic value", number);
                                    args[Factory<Hub>::resolveKey("y", args)].value = forEx;
                                    // assigning forEx to prevent crashing by an std::bad_cast<>
                                }
                            }
                            auto _hub = new Hub(Factory<Hub>::create(args));
                            this->_map.addHub(_hub);
                            bool dupLoc = std::any_of(this->_map.hubs.begin(), this->_map.hubs.end(),
                                                      [_hub](Hub const *hub) -> bool 
                                                      {
                                                        return hub != _hub
                                                            && _hub->position_x == hub->position_x 
                                                            && _hub->position_y == hub->position_y;
                                                      });
                            if (dupLoc)
                            {
                                this->validator->saveError(FailureCause::DuplicateHubLoc,
                                                           "regarding " + _hub->name, number);
                                continue;
                            }
                            if (std::any_of(this->_map.hubs.begin(), this->_map.hubs.end(),
                                [_hub](Hub const *hub) -> bool { return hub != _hub
                                                                 && hub->name == _hub->name; }))
                            {
                                this->validator->saveError(FailureCause::DuplicateHubName,
                                                           _hub->name, number);
                                continue;
                            }
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
                            if (_hub->position_x < 0)
                            {
                                this->validator->saveError(FailureCause::IncorrectLocValue, "x-coord cannot be negative", number);
                                continue;
                            }

                        }
                        else if(key == "connection")
                        {
                            auto args = Factory<Connection>::ready_args();
                            std::vector<std::string> values = split(row, ' ');
                            values.erase(values.begin()); // necessary to cut out empty records.
                            if (values.empty() || values.size() > 2)
                            {
                                this->validator->saveError(FailureCause::InvalidConnection,
                                    "Connection definition must consist out of <hub_name>-<hub_name> [...]",
                                    number);
                                args.clear();
                                continue;
                            }
                            std::vector<std::string> hubs = split(values[0], '-');
                            if (hubs.size() != 2)
                            {
                                this->validator->saveError(FailureCause::ImpossibleConnection,
                                    "two hubs ONLY are needed", number);
                                args.clear();
                                continue;
                            }
                            if (this->_map.getHub(hubs[0]) == nullptr
                             || this->_map.getHub(hubs[1]) == nullptr)
                            {
                                if (this->_map.getHub(hubs[0]) == nullptr)
                                    this->validator->saveError(FailureCause::InvalidConnection,
                                    "hub '"+hubs[0]+"' is non-existent", number);

                                if (this->_map.getHub(hubs[1]) == nullptr)
                                    this->validator->saveError(FailureCause::InvalidConnection,
                                    "hub '"+hubs[1]+"' is non-existent", number);
                                continue;
                            }
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
                                    {
                                        this->validator->saveError(
                                            FailureCause::IncorrectMetaFormation,
                                            "Connection row only expects one optional record",
                                            number);
                                        continue;
                                    }
                                    std::string _key = metaKeyValue[0].erase(0, 1);
                                    std::string value = metaKeyValue[1].erase(metaKeyValue[1].length() - 1, 1);
                                    int insert = Factory<Connection>::resolveKey(_key, args);
                                    if (insert != -1)
                                        args[insert].value = value;
                                }
                                else if (values[1].find('[') == std::string::npos ||
                                         values[1].find(']') == std::string::npos)
                                {
                                    this->validator->saveError(
                                        FailureCause::IncompleteMetaClosure,
                                        "Custom brackets enclosed incorrectly",
                                        number);
                                    continue;
                                }
                            }
                            auto connection = new Connection(Factory<Connection>::create(args));
                            bool dupCon = std::any_of(this->_map.connections.begin(),
                                                      this->_map.connections.end(),
                                                     [connection](Connection const *conn) -> bool {
                                                        return connection != conn
                                                            && connection->hub1 == conn->hub1
                                                            && connection->hub2 == conn->hub2;
                                                     });
                            if (dupCon)
                            {
                                this->validator->saveError(FailureCause::DuplicateConnection,
                                        "("+connection->hub1->name+"-"+connection->hub2->name+")", number);
                                delete connection;
                                continue;
                            }
                            this->_map.addConnection(connection);
                            ExecuteState connect_result = connection->validate_connection(this->_map);
                            if (!connect_result.success)
                                throw ParseException(connect_result.why.value(), forEx);
                        }
                        else if(key == "nb_drones")
                        {
                            std::string fromRow = row;
                            while (!fromRow.empty() && fromRow[0] == 32)
                                fromRow.erase(0, 1);
                            if (!isNumeric(fromRow))
                            {
                                this->validator->saveError(FailureCause::InvalidNbDrones,
                                        "must be numeric", number);
                                this->nb_drones = 1;
                            }
                            else if (stoi(fromRow) <= 0)
                            {
                                this->validator->saveError(FailureCause::InvalidNbDrones,
                                        "value cannot be negative or zero", number);
                                this->nb_drones = 1;
                            }
                            else
                                this->nb_drones = stoi(fromRow);
                        }
                        else
                        {
                            this->validator->saveError(FailureCause::UnfamiliarKeyFound,
                                "'"+key+"'", number);
                            continue;
                        }
                    }
                }
            }
            if (start == 0 || end == 0 || this->nb_drones < 1)
            {
                if (start == 0)
                    this->validator->saveError(
                        FailureCause::NoStartHubFound,
                        "start_hub is required");
                if (end == 0)
                    this->validator->saveError(
                        FailureCause::NoEndHubFound,
                        "end_hub is required");
                if (this->nb_drones < 1) 
                    this->validator->saveError(
                        FailureCause::MissingNbDrones, 
                        "nb_drones is required");
            }
            for (auto &con : this->_map.connections)
            {
                delete con;
                con = nullptr;
            }
            if (!this->validator->isPassed())
                throw ParseException(TextFormat("could not parse '%s'", this->_map.name));
            return ExecuteState::Ok("Ok");
        }
        catch (ParseException &pex)
        {
            auto errors = this->validator->releaseErrors();
            return ExecuteState::Fail(pex.what(), 0, std::any_cast<std::vector<std::string>>(errors));
        }
        catch (std::exception &ex)
        {
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
    if (!mapResult.success)
    {
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