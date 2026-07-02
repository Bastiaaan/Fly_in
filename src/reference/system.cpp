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
}

System::~System()
{
    cout << "shutting down" << endl;
}

int System::Init(int level, std::string &difficulty)
{
    cout << "" << endl;

    return 1;
}

void System::RegisterMap()
{
    cout << "RegisterMap() is not implemented yet" << endl;
}

vector<map<int, string>> System::get_options(const std::string &difficulty)
{
    vector<int> options;
    int opt = 0;
    string currPath = filesystem::current_path().string();
    regex pattern(R"(^\d{1,2}_[a-z]+(?:_[a-z]+)*\.txt$)");
    vector<string> assemblies = split(currPath, '/', 0);
    unsigned int fly_in = 0;
    while (assemblies[fly_in] != "fly_in")
        fly_in++;
    fly_in++;
    string _core = join(assemblies, '/', fly_in);
    _core += "/maps/" + difficulty + '/';
    for (const auto& fileSrc : filesystem::directory_iterator(_core))
    {
        vector<string> broken = split(fileSrc.path().string(), '/');
    }
    return options;
}