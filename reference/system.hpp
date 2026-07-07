
#ifndef SYSTEM_HPP
# define SYSTEM_HPP

#include "fly_in.hpp"
#include "map.hpp"

using namespace std;

class System
{
	private:
		int nb_drones;
        string difficulty;
        int level;
        string mapsBasePath();
		
	public:
		int score;
		int turns;
		std::string mapSrc;
        Map _map;
        int Load(std::string &level, std::string &difficulty);
        vector<tuple<int, string, string>> get_options(const std::string &difficulty);
        System();
        ~System();
};

#endif