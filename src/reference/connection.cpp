//
// Created by brogaar on 7/6/26.
//

#include "./../../reference/fly_in.hpp"

Connection::Connection()
{

}

Connection::~Connection()
{
    delete this->hub1;
    delete this->hub2;
}

void Connection::setHub1(Map &map, const std::string &hub1)
{
    auto byName = [&map](string const &hub_name)-> Hub* {
        for (auto &hub : map.hubs)
        {
            if (hub.name == hub_name)
                return &hub;
        }
        return nullptr;
    };
    this->hub1 = byName(hub1);
}

void Connection::setHub2(Map &map, std::string const &hub2)
{
    auto byName = [&map](string const &hub_name)-> Hub* {
        for (auto &hub : map.hubs)
        {
            if (hub.name == hub_name)
                return &hub;
        }
        return nullptr;
    };
    this->hub2 = byName(hub2);
}

void Connection::setMlc(optional<int> mlc)
{
    this->max_link_capacity = mlc;
}

ExecuteState Connection::validate_connection(Map &map)
{
    ExecuteState state;
    return state;
}