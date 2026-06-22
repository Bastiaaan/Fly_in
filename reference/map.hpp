
#include <vector>
#include "hub.hpp"

class Map {
	public:
	std::vector<Hub> hubs;
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