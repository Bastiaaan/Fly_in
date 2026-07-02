
#ifndef HUB_HPP
# define HUB_HPP

#include "drone.hpp"
#include "fly_in.hpp"
#include "map.hpp"

using namespace std;

class Hub {
	public:
		vector<Drone> drones;
		int position_x;
		int position_y;
		string name;
		Hub *next;
		Hub *prev;
		Map map;
		Hub(Map map, int x, int y, string name);
		~Hub();
};

#endif
