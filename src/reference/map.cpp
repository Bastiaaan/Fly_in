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

Connection* Map::getConnection(std::string const &hub1, std::string const &hub2)
{
    auto const _hub1 = getHub(hub1);
    auto const _hub2 = getHub(hub2);

    std::vector<Connection*> foundConnections;
    std::copy_if(
        this->connections.begin(),
        this->connections.end(),
        std::back_inserter(foundConnections),
        [_hub1, _hub2]
        (Connection const *conn) -> bool {
            return conn->hub1 == _hub1 && conn->hub2 == _hub2; 
        }
    );
    return foundConnections.size() == 0 ? nullptr : foundConnections[0];
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
        delete connection;
        connection = nullptr;
    }
    this->connections.clear();

    for (auto hub : this->hubs)
    {
        delete hub;
        hub = nullptr;
    }
    this->hubs.clear();
}
