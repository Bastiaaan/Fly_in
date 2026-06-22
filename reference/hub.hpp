
#include <string> 
#include "map.hpp"
#include "drone.hpp"

class Hub {
	public:
		std::vector<Drone> drones;
		int position_x;
		int position_y;
		std::string name;
		this connection;
		Hub(int x, int y, std::string name);
		~Hub();
};

Hub::Hub(int x, int y, std::string name) {
	this->position_x = x;
	this->position_y = y;
	this->name = name;
}
