//
// Created by brogaar on 30-6-26.
//

#include "../../reference/fly_in.hpp"

using namespace std;

System::System()
{
    this->nb_drones = 0;
    this->score = 0;
    this->difficulty = "TBD";
    this->turns = 0;
    this->_map = Map();
}

System::~System()
{
    cout << endl << "== shutting down ==" << endl;
}

string System::mapsBasePath()
{
    string currentPath = filesystem::current_path().string();
    vector<string> parts = split(currentPath, '/', 0);
    unsigned int fly_in = 0;
    while (parts[fly_in] != "fly_in")
        fly_in++;
    fly_in++;
    string _core = join(parts, '/', fly_in);
    return _core + "/maps/";
}

int System::Load(std::string &level, std::string &difficulty)
{
    cout << "Initializing Fly-in..." << endl;
    unsigned int line = 1;
    string target = this->mapsBasePath() + difficulty + '/' + level;
    ifstream _map(target);
    if (!_map.is_open())
        cerr << "ERROR opening '" << level << "'" << endl;
    string buffer;
    vector<Hub> collectedHubs;
    vector<Connection> collectedConnections;
    while (getline(_map, buffer))
    {
        if (buffer.length() > 0)
        {
            if (buffer[0] != '#')
            {
                vector<string> keyRow = split(buffer, ':');
                if (keyRow.size() != 2) {
                    cerr << "Config error at line " << line << ". Each row should only contain one double column"
                         << endl;
                    _map.close();
                    return 0;
                }
                string key = keyRow[0];
                string row = keyRow[1];
                if (key == "start_hub" || key == "end_hub" || key == "hub")
                {
                    cout << "hub to be made" << endl;
                    vector<string> values = split(row, ' ');
                    tuple<string, int, int, optional<string>,optional<string>,optional<int>> args;
                    unsigned int argPos = 0;
                    //for ()
                }
                else if(key == "connection")
                {
                    cout << "connection to be established" << endl;
                    tuple<string, string, optional<int>> args;
                }
                else if(key == "nb_drones")
                    this->nb_drones = stoi(row);
                else
                {
                    cerr << "Unknown config key of " << key << "found at line " << line << endl;
                    _map.close();
                    return 0;
                }
            }
        }
        line++;
    }
    _map.close();
    return 1;
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