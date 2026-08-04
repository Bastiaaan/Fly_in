
#pragma once
# include "drone.hpp"

class Connection;

enum Zone
{
    Normal = 0,
    Priority = 1,
    Restricted = 2,
    Blocked = 3
};

class Hub {
    std::optional<int> max_drones;
    bool start;
    bool end;
	public:
		Hub() = default;
		~Hub() = default;
		std::string name;
		int position_x;
		int position_y;
		std::optional<std::string> color;
        std::vector<Drone*> drones;
		std::optional<Zone> zone;
		Hub* connected;
        void setName(std::string name);
        void setX(int x);
        void setY(int y);
        void setZone(std::optional<std::string> const &zoneName);
        void setColor(std::optional<std::string> const &color);
        void setMaxDrones(std::optional<int> max_drones);
		void setStartOrEnd(std::string const &soe);
		bool isStart() const;
		bool isEnd() const;
        void dropInfo() const;
};


