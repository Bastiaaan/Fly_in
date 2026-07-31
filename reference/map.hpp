
# pragma once
class Connection;
# include "connection.hpp"
# include "hub.hpp"


using namespace std;

class Map {
    string srcPath;
	public:
        string name;
        string difficulty;
        vector<Hub*> hubs;
        vector<Connection*> connections;
		vector<Drone*> available_drones;
        void addHub(Hub* hub);
        void addConnection(Connection* connection);
        void setName(string &name);
        void setDifficulty(string &difficulty);
        void setSrcPath(string &path);
		Hub* getHub(string const &hubName);
        Map() = default;
	    Map(string &name,
            string &srcPath,
            string &difficulty);
	    ~Map();
};
