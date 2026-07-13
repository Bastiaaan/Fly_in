
#ifndef HUB_HPP
# define HUB_HPP

#include "drone.hpp"
#include "fly_in.hpp"

using namespace std;

enum Zone
{
    Normal = 0,
    Priority = 1,
    Restricted = 2,
    Blocked = 3
};

class Hub {
	public:
        optional<string> color;
        optional<int> max_drones;
        optional<Zone> zone;
        vector<Drone> drones;
		int position_x;
		int position_y;
		string name;
		Hub(string const &name,
            int x,
            int y,
            optional<string> &zone,
            optional<string> &color,
            optional<int> max_drones);
		~Hub();
};

class HubData : public Data<Hub>
{
	public:
		HubData();
		~HubData();
	private:
		string name;
		int x;
		int y;
		optional<string> zone;
		optional<string> color;
		optional<int> max_drones;
};

#endif
