
#ifndef MAP_HPP
# define MAP_HPP

#include "fly_in.hpp"
#include "hub.hpp"

using namespace std;

class Map {
    string srcPath;
	public:
        string name;
        string difficulty;
        vector<Hub> hubs;
        void add(Hub& hub);
        void setName(string &name);
        void setDifficulty(string &difficulty);
        void setSrcPath(string &path);
        Map();
	    Map(string &name,
            string &srcPath,
            string &difficulty);
	    ~Map();
};

#endif