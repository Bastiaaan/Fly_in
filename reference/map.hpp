
#ifndef MAP_HPP
# define MAP_HPP

#include <string>
#include <vector>
#include "hub.hpp"

class Map {
	public:
	std::vector<Hub> hubs;
	std::string srcPath;
	Map();
	~Map();
};

Map::Map()
{

};

Map::~Map()
{
	std::cout << "decon.." << std::endl;
}

#endif