//
// Created by brogaar on 7/6/26.
//

#include "./../../reference/fly_in.hpp"

void Connection::setHub1(Hub *hub1)
{
    this->hub1 = hub1;
}

void Connection::setHub2(Hub *hub2)
{
    this->hub2 = hub2;
}

void Connection::setMlc(optional<int> max_link_capacity)
{
    this->max_link_capacity = max_link_capacity;
}

void Connection::establish() const
{
    Link *connection = new Link();
    connection->hub = hub2;
    if (this->max_link_capacity.has_value())
        connection->max_link_capacity = this->max_link_capacity.value();
    else
        connection->max_link_capacity = std::nullopt;
    this->hub1->connections[this->hub2->name] = connection;
}

ExecuteState Connection::validate_connection(Map &map) const
{
    auto realConnection = [&map](Hub const *hub1, Hub const *hub2) -> bool
    {
        for (auto const &conn : map.connections)
        {
            if (conn->hub1->name == hub1->name &&
                conn->hub2->name == hub2->name)
                return true;
        }
        return false;
    };
    try
    {
        if (!realConnection(this->hub1, this->hub2))
            throw std::logic_error("Connection not found");
        this->establish();
        return ExecuteState::Ok("Connection established");
    }
    catch (std::exception &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}
