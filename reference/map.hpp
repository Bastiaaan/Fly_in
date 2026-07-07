
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
        Map();
	    Map(string &name,
            string &srcPath,
            string &difficulty);
	    ~Map();
        void add(Hub& hub);
};

#endif