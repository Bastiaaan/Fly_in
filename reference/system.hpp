#pragma once

#include <any>
#include <optional>
#include "map.hpp"

using namespace std;

class Validator;

struct ExecuteState
{
    bool success = false;
    std::any result;
    std::optional<unsigned int> line;
    std::optional<std::string> why;
	static ExecuteState Fail(std::string const &reason, unsigned int line = 0, std::any result = std::nullopt)
	{
		ExecuteState s;
		s.success = false;
		if (result.has_value())
			s.result = result;
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
	Drone const *drone;
	Hub const *src;
	Hub const *dest;
	int drones_moved;
	int max_link_capacity;
	std::string _output;
	static Log* output(Drone const &d, Hub const &src, Hub const &dest, int drones_moved, int max_link_capacity)
	{
		auto log = new Log();
		log->drone = &d;
		log->drones_moved = drones_moved;
		log->max_link_capacity = max_link_capacity;
		log->src = &src;
		log->dest = &dest;
		log->_output = TextFormat("D%d-%s", d.id, dest.name.c_str());
		return log;
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
		bool is_graphic;
        unsigned long nb_drones;
		bool verbose_log;
		std::string mapSrc;
        Map _map;
		std::map<int, std::vector<Log*>> logs;
		void verboseLog(int const n);
        Validator *validator;
		ExecuteState initDrones();
		ExecuteState registerTurn();
		ExecuteState jsonMap();
		ExecuteState extractMap(std::string const &path);
		const Hub& findByCoordinates(int x, int y);
        static void verboseFree(ExecuteState const &ex);
        System();
        ~System();
};
