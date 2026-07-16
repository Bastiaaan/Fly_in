
#ifndef HUB_HPP
# define HUB_HPP

#include "fly_in.hpp"
#pragma once

using namespace std;

enum Zone
{
    Normal = 0,
    Priority = 1,
    Restricted = 2,
    Blocked = 3
};

class Hub {
    optional<string> color;
    optional<int> max_drones;
    optional<Zone> zone;
    int position_x;
    int position_y;
	public:
		Hub();
		~Hub();
        string name;
        vector<Drone> drones;
        void setName(string name);
        void setX(int x);
        void setY(int y);
        void setZone(optional<string> zoneName);
        void setColor(optional<string> color);
        void setMaxDrones(optional<int> max_drones);

};

#endif
