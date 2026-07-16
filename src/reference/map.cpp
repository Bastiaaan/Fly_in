//
// Created by brogaar on 29-6-26.
//

#include "../../reference/fly_in.hpp"

void Map::add(Hub &hub)
{
    try
    {
        this->hubs.push_back(hub);
    }
    catch(exception ex)
    {
        cerr << "could not add hub" << endl;
    }
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

void Map::setName(string &name)
{
    this->name = name;
}

void Map::setDifficulty(string &difficulty)
{
    this->difficulty = difficulty;
}

void Map::setSrcPath(string &path)
{
    this->srcPath = path;
}
