//
// Created by brogaar on 29-6-26.
//

#include "fly_in.hpp"

void Map::addHub(Hub *hub)
{
    try
    {
        this->hubs.push_back(hub);
    }
    catch(std::exception &ex)
    {
        cerr << "could not add hub" << endl;
    }
}

Hub* Map::getHub(string const &hubName)
{
    auto _find = [this](std::string const &_name) -> Hub*
    {
        for (auto const found : this->hubs)
        {
            if (found->name == _name)
                return found;
        }
        return nullptr;
    };
    return _find(hubName);
}

void Map::addConnection(Connection *connection)
{
    try
    {
        this->connections.push_back(connection);
    }
    catch (std::exception &ex)
    {
        cerr << "could not add a new connection: " << ex.what() << endl;
    }
}

Map::Map(string &name,
         string &srcPath,
         string &difficulty)
{
    this->name = name;
    this->srcPath = srcPath;
    this->difficulty = difficulty;
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

Map::~Map()
{
    for (auto connection : this->connections)
    {
        std::cout << "disconnecting " << connection->hub1->name << '-' << connection->hub2->name << std::endl;
        delete connection;
        connection = nullptr;
    }
    this->connections.clear();

    for (auto hub : this->hubs)
    {
        std::cout << "removing " << hub->name << std::endl;
        delete hub;
        hub = nullptr;
    }
    this->hubs.clear();
}
