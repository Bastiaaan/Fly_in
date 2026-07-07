//
// Created by brogaar on 29-6-26.
//

#include "../../reference/fly_in.hpp"

void Map::add(Hub &hub)
{
    this->hubs.push_back(hub);
}

Map::Map()
{

}

Map::Map(string &name,
         string &srcPath,
         string &difficulty)
{
    this->name = name;
    this->srcPath = srcPath;
    this->difficulty = difficulty;
}

Map::~Map()
{
    cout << "decon.." << endl;
}
