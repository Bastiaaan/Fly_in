
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
        static string mapsBasePath();
		
	public:
		int score;
		int turns;
		std::string mapSrc;
        Map _map;
        int Load(std::string const &level, std::string const &difficulty);
        vector<tuple<int, string, string>> get_options(const std::string &difficulty);
        System();
        ~System();
};

#endif