//
// Created by brogaar on 30-6-26.
//

#include <any>
#include <optional>
#include "../../reference/fly_in.hpp"
#include "factory.tpp"

using namespace std;

System::System()
{
    this->nb_drones = 0;
    this->level = 0;
    this->score = 0;
    this->difficulty = "TBD";
    this->turn = 0;
    this->_map = Map();
}

System::~System()
{
    cout << endl << "== shutting down ==" << endl;
}

void System::verboseFree(ExecuteState const &ex)
{
    if (!ex.success)
    {
        if (ex.line != nullopt)
            cout << "ERROR " << std::any_cast<string>(ex.why) << endl << "At line " << std::any_cast<int>(ex.line) << endl;
        else
            cout << "ERROR: " << std::any_cast<string>(ex.why) << endl;
    }
    else
        cout << "No errors found" << endl;
}

ExecuteState System::mapsBasePath()
{
    try
    {
        string const currentPath = filesystem::current_path().string();
        vector<string> const parts = split(currentPath, '/');
        unsigned int fly_in = 0;
        while (parts[fly_in] != "fly_in")
            fly_in++;
        fly_in++;
        string const _core = join(parts, '/', fly_in);
        string const &result = _core + "/maps/";
        return ExecuteState::Ok(result);
    }
    catch (exception &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}

ExecuteState System::Load(std::string const &level, std::string const &difficulty)
{
    auto state = new ExecuteState();
    cout << "Initializing Fly-in..." << endl;
    unsigned int line = 1;
    auto mapResult = mapsBasePath();
    if (mapResult.success != false)
    {
        auto result = mapResult.result;
        string const base = any_cast<string>(result);
        string const target = base + difficulty + '/' + level;
        ifstream _mapped(target);
        if (!_mapped.is_open())
        {
            return ExecuteState::Fail("Could not open map '" + target + "'\n", line);
        }
        string buffer;
        vector<Connection> collectedConnections;
        while (getline(_mapped, buffer))
        {
            if (!buffer.empty())
            {
                if (buffer[0] != '#')
                {
                    vector<string> keyRow = split(buffer, ':');
                    if (keyRow.size() != 2) {
                        _mapped.close();
                        return ExecuteState::Fail("Each row should only contain one double column\n", line);
                    }
                    string const key = keyRow[0];
                    string const row = keyRow[1];
                    if (key == "start_hub" || key == "end_hub" || key == "hub")
                    {
                        vector<string> values = split(row, ' ');
                        unsigned int opened_meta = 0;
                        int pos = 0;
                        values.erase(values.begin());
                        map<int, Argument> args = Factory<Hub>::ready_args();
                        for (const auto &rec : values)
                        {
                            if (rec.find('[') != string::npos)
                                opened_meta++;
                            if (opened_meta > 0)
                            {
                                vector<string> meta_pairs = split(rec, '=');
                                if (meta_pairs.size() != 2)
                                {
                                    _mapped.close();
                                    return ExecuteState::Fail("unusual assignments found\n", line);
                                }
                                string metaName = meta_pairs[0];
                                string metaValue = meta_pairs[1];
                                if (metaName.find('[') != string::npos && metaName[0] == '[')
                                    metaName.erase(0, 1);
                                if (metaValue.find(']') != string::npos && metaValue[metaValue.size() - 1] == ']')
                                    metaValue.erase(metaValue.size() - 1, 1);
                                if (args[pos].key != metaName)
                                {
                                    while (args[pos].key != metaName)
                                        pos++;
                                }
                                args[pos].value = metaValue;
                                if (rec.find(']') != string::npos)
                                    opened_meta--;
                            }
                            else
                            {
                                args[pos].value = rec;
                                pos++;
                            }
                        }
                        if (opened_meta != 0) {
                            _mapped.close();
                            return ExecuteState::Fail("Brackets must be enclosed.\n", line);
                        }
                        Hub _hub = Factory<Hub>::create(args);
                        this->_map.hubs.push_back(_hub);
                        cout << endl;
                    }
                    else if(key == "connection")
                    {
                        auto args = Factory<Connection>::ready_args();
                        vector<string> values = split(row, ' ');
                        values.erase(values.begin()); // necessary to cut out empty records.
                        if (values.empty() || values.size() > 2)
                        {
                            _mapped.close();
                            return ExecuteState::Fail("Connection definition must consist out of <hub_name>-<hub_name> [...]\n", line);
                        }
                        vector<string> hubs = split(values[0], '-');
                        if (hubs.size() != 2)
                        {
                            _mapped.close();
                            return ExecuteState::Fail("Invalid connection definition detected\n", line);
                        }
                        Map map = this->_map;
                        auto getByHubName = [&map](std::string const &hubName) -> Hub*
                        {
                            for (auto& h : map.hubs)
                            {
                                if (h.name == hubName)
                                    return &h;
                            }
                            return nullptr;
                        };
                        if (getByHubName(hubs[0]) == nullptr)
                        {
                            _mapped.close();
                            return ExecuteState::Fail("First hub is not found\n", line);
                        }
                        if (getByHubName(hubs[1]) == nullptr)
                        {
                            _mapped.close();
                            return ExecuteState::Fail("Second hub is not found\n", line);
                        }

                        args[0].value = getByHubName(hubs[0]);
                        args[1].value = getByHubName(hubs[1]);
                        cout << "how many values in this connection: " << values.size() << endl;
                        auto connection = Factory<Connection>::create(args);

                    }
                    else if(key == "nb_drones")
                        this->nb_drones = stoi(row);
                    else
                    {
                        _mapped.close();
                        return ExecuteState::Fail("Unknown config key found.\n", line);
                    }
                }
            }
            line++;
        }
        _mapped.close();
        state->success = true;
        return ExecuteState::Ok("parsed");
    }
    return mapResult;
}

vector<tuple<int, string, string>> System::get_options(const std::string &difficulty)
{
    cout << "choosing out of options..." << endl;
    vector<tuple<int, string, string>> options; // <level, display_name, file>
    int opt = 0;
    regex pattern(R"(^\d{1,2}_[a-z]+(?:_[a-z]+)*\.txt$)");
    auto const mapResult = mapsBasePath();
    if (mapResult.success == false) {
        verboseFree(mapResult);
        return options;
    }
    string const _core = std::any_cast<string>(mapResult.result) + difficulty + '/';
    for (const auto& fileSrc : filesystem::directory_iterator(_core))
    {
        opt++;
        vector<string> broken = split(fileSrc.path().string(), '/');
        string fileName = broken[broken.size() - 1];
        vector fileParts = split(split(fileName, '.')[0], '_');
        fileParts.erase(fileParts.begin());
        options.push_back({opt, join(fileParts, ' '), fileName});
    }
    return options;
}