//
// Created by brogaar on 7/6/26.
//

#include "./../../reference/fly_in.hpp"

Connection::Connection()
{

}

Connection::~Connection()
{

}

void Connection::setHub1(Hub *hub1)
{
    this->hub1 = hub1;
}

void Connection::setHub2(Hub *hub2)
{
    this->hub2 = hub2;
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