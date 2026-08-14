
#pragma once
# include "drone.hpp"
# include "fly_in.hpp"

struct HubPoint;
struct Link;
class Connection;

enum Zone
{
    Normal = 0,
    Priority = 1,
    Restricted = 2,
    Blocked = 3
};

class Hub {
	public:
		Hub();
		~Hub();
		std::optional<int> max_drones;
		bool start;
		bool end;
		std::string name;
		int position_x;
		int position_y;
		std::optional<std::string> color;
        std::vector<Drone*> drones;
		std::optional<Zone> zone;
		std::map<std::string, Link*> connections;
		HubPoint *location;
        void setName(std::string const &name);
        void setX(int x);
        void setY(int y);
        void setZone(std::optional<std::string> const &zoneName);
        void setColor(std::optional<std::string> const &color);
        void setMaxDrones(std::optional<int> max_drones);
		void setStartOrEnd(std::string const &soe);
		void transferDrone(Link &connection, float hubRadius);
		bool isStart() const;
		bool isEnd() const;
        void dropInfo() const;
};

struct HubPoint
{
	Hub &hub;
	unsigned long x, y;
	static HubPoint* Save(Hub& hub, unsigned long x, unsigned long y) {
		return new HubPoint { hub, x, y };
	}
};

struct Link
{
	Hub *hub;
	std::optional<int> max_link_capacity;
};
