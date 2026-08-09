
#pragma once

#include <any>
#include "map.hpp"
#include "validator.hpp"

using namespace std;

class Validator;

struct ExecuteState
{
    bool success = false;
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
	static ExecuteState Ok(std::optional<std::any> const &result)
	{
		ExecuteState s;
		s.success = true;
        if (result.has_value())
		    s.result = result.value();
        else
            s.result = "Ok";
		return s;
	}
};

struct Option
{
	std::string difficulty;
	std::string name;
	int level;
};

struct Log
{
	Drone &drone;
	Hub &hub;
	std::string output;
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
		std::vector<Log> logs;
        Validator *validator;
		ExecuteState initDrones();
		ExecuteState rotateDrone();
		ExecuteState registerTurn();
		const Hub& findByCoordinates(int x, int y);
        ExecuteState Load(std::string const &level, std::string const &difficulty);
        vector<tuple<int, string, string>> get_options(const std::string &difficulty);
        static void verboseFree(ExecuteState const &ex);
        System();
        ~System();
};
