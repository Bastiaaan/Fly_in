
#ifndef SYSTEM_HPP
# define SYSTEM_HPP

#include "map.hpp"
#include <string> 

class System
{
	private:
		std::string map_path;
		int nb_drones;
        std::string difficulty;
        int level;
		
	public:
		int score;
		int turns;
		std::string pathToMap;
		System();
        ~System();
        void RegisterMap();
        int Setup();
};

System::System()
{
	this->score = 0;
	this->turns = 0;
}

int System::Setup()
{
    int status = 0;
    std::cout << ""
    return status;
}

#endif