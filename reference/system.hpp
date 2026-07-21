
#ifndef SYSTEM_HPP
# define SYSTEM_HPP

#include <any>
#include "fly_in.hpp"
#include "map.hpp"

using namespace std;

struct ExecuteState
{
    bool success = NULL;
    std::any result;
    optional<unsigned int> line;
    optional<std::string> why;
	static ExecuteState Fail(std::string const &reason, unsigned int line = -1)
	{
		ExecuteState s;
		s.success = false;
		if (line > 0)
			s.line = line;
		s.why = reason;
		return s;
	}
	static ExecuteState Ok(std::any const &result)
	{
		ExecuteState s;
		s.success = true;
		s.result = result;
		return s;
	}
};

class System
{
	private:
        string difficulty;
        int level;
        static ExecuteState mapsBasePath();
		
	public:
		int score;
		int turn;
        int nb_drones;
		std::string mapSrc;
        Map _map;
        ExecuteState Load(std::string const &level, std::string const &difficulty);
        vector<tuple<int, string, string>> get_options(const std::string &difficulty);
        static void verboseFree(ExecuteState const &ex);
        System();
        ~System();
};

#endif