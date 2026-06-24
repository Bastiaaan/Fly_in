
#ifndef HUB_HPP
# define HUB_HPP

#include <string> 
#include <vector>
#include "map.hpp"
#include "drone.hpp"

class Hub {
	public:
		std::vector<Drone> drones;
		int position_x;
		int position_y;

		std::string name;
		Map map;
		Hub(Map map, int x, int y, std::string name);
		~Hub();
};

class Connection {
	public:
		Hub origin;
		Hub destination;
		std::string display()
};

enum Zone {
	normal = 0,
	blocked = 1,
	restricted = 2,
	priority = 3,
};

Hub::Hub(Map &map, int x, int y, std::string name)
{
	this->map = map;
	this->position_x = x;
	this->position_y = y;
	this->name = name;
}

Hub::~Hub()
{

}

#endif
