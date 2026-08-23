# pragma once

#include "hub.hpp"
#include "connection.hpp"
#include "drone.hpp"

class Connection;


class Map {
    std::string srcPath;
	public:
        std::string name;
        std::string difficulty;
        std::vector<Hub*> hubs;
        std::vector<Connection*> connections;
		std::vector<Drone*> available_drones;
        void addHub(Hub* hub);
        void addConnection(Connection* connection);
        void setName(std::string &name);
        void setDifficulty(std::string &difficulty);
        void setSrcPath(std::string &path);
		Hub* getHub(std::string const &hubName);
        Connection* getConnection(std::string const &hub1, std::string const &hub2);
        Map() = default;
	    Map(std::string &name,
            std::string &srcPath,
            std::string &difficulty);
	    ~Map();
};
