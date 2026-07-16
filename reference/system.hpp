
#ifndef SYSTEM_HPP
# define SYSTEM_HPP

#include "fly_in.hpp"
#include "map.hpp"

using namespace std;

using Result = variant<int, string, bool, char>;

struct ExecuteState
{
    bool success;
    optional<Result> result;
    optional<int> line;
    optional<std::string> why;
};

class System
{
	private:
        string difficulty;
        int level;
        static string mapsBasePath();
		
	public:
		int score;
		int turn;
        int nb_drones;
		std::string mapSrc;
        Map _map;
        ExecuteState* Load(std::string const &level, std::string const &difficulty);
        vector<tuple<int, string, string>> get_options(const std::string &difficulty);
        void verbose_state(ExecuteState *ex);
        System();
        ~System();
};

#endif