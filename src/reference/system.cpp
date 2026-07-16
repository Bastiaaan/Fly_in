//
// Created by brogaar on 30-6-26.
//

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

void System::verbose_state(ExecuteState *ex)
{
    if (!ex->success)
    {
        cout << "ERROR: " << (*ex->why);
    }
}

string System::mapsBasePath()
{
    try
    {
        string currentPath = filesystem::current_path().string();
        vector<string> parts = split(currentPath, '/');
        unsigned int fly_in = 0;
        while (parts[fly_in] != "fly_in")
            fly_in++;
        fly_in++;
        string const _core = join(parts, '/', fly_in);
        return _core + "/maps/";
    }
    catch (exception ex)
    {

        return "";
    }
}

ExecuteState* System::Load(std::string const &level, std::string const &difficulty)
{
    auto state = new ExecuteState();
    cout << "Initializing Fly-in..." << endl;
    unsigned int line = 1;
    string target = this->mapsBasePath() + difficulty + '/' + level;
    ifstream _map(target);
    if (!_map.is_open())
    {
        state->success = false;
        state->why = "Could not open map '" + target + "'\n";
        return state;
    }
    string buffer;
    vector<Connection> collectedConnections;
    while (getline(_map, buffer))
    {
        if (!buffer.empty())
        {
            if (buffer[0] != '#')
            {
                vector<string> keyRow = split(buffer, ':');
                if (keyRow.size() != 2) {
                    state->line = line;
                    state->success = false;
                    state->why = "Each row should only contain one double column\n";
                    _map.close();
                    return state;
                }
                string key = keyRow[0];
                string row = keyRow[1];
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
                                state->line = line;
                                state->success = false;
                                state->why = "unusual assignments found\n";
                                _map.close();
                                return state;
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
                        state->line = line;
                        state->success = false;
                        state->why = "Brackets must be enclosed.";
                        _map.close();
                        return state;
                    }
                    Hub result = Factory<Hub>::create(args);
                    this->_map.add(result);
                    cout << endl;
                }
                else if(key == "connection")
                {
                    vector<string> values = split(row, ' ');
                    values.erase(values.begin()); // necessary to cut out empty records.
                    if (values.size() < 1 || values.size() > 2)
                    {
                        state->line = line;
                        state->success = false;
                        state->why = "Connection definition must consist out of <hub_name>-<hub_name> [...]\n";
                        _map.close();
                        return state;
                    }
                    vector<string> hubs = split(values[0], '-');
                    if (hubs.size() != 2)
                    {
                        state->line = line;
                        state->success = false;
                        state->why = "Invalid connection definition detected\n";
                        _map.close();
                        return state;
                    }
                    else
                    {
                        Map map = this->_map;
                        auto name = [map](std::string hubName) -> Hub*
                        {
                            for (Hub _h : map.hubs)
                            {
                                if (_h.name == hubName)
                                    return &_h;
                            }
                            return nullptr;
                        };
                        if (name(hubs[0]) != nullptr)
                        {

                        }
                    }
                }
                else if(key == "nb_drones")
                    this->nb_drones = stoi(row);
                else
                {
                    state->success = false;
                    state->why = "Unknown config key found.";
                    state->line = line;
                    _map.close();
                    return state;
                }
            }
        }
        line++;
    }
    _map.close();
    state->success = true;
    return state;
}

vector<tuple<int, string, string>> System::get_options(const std::string &difficulty)
{
    vector<tuple<int, string, string>> options; // <level, display_name, file>
    int opt = 0;
    regex pattern(R"(^\d{1,2}_[a-z]+(?:_[a-z]+)*\.txt$)");
    string _core = this->mapsBasePath() + difficulty + '/';
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