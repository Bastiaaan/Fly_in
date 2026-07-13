//
// Created by brogaar on 7/6/26.
//

#include "./../../reference/fly_in.hpp"

Connection::Connection(std::string const &hub1, std::string const &hub2, std::optional<int> mlc)
{
    this->conn_name = hub1 + '_' + hub2;
    this->max_link_capacity = mlc != std::nullopt? mlc : std::nullopt;
}

Connection::~Connection()
{

}