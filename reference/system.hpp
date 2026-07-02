
#ifndef SYSTEM_HPP
# define SYSTEM_HPP

#include "fly_in.hpp"
#include "map.hpp"

using namespace std;

class System
{
	private:
		int nb_drones;
        std::string difficulty;
        int level;
		
	public:
		int score;
		int turns;
		std::string mapSrc;
		System();
        ~System();
        void RegisterMap();
        int Init(int level, std::string &difficulty);
        vector<map<int, string>> get_options(const std::string &difficulty);
		bool preCheckMaps();
};

#endif