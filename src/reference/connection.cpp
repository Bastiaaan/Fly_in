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
        return ExecuteState::Ok("Connection established");
    }
    catch (std::exception &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}

ExecuteState Connection::drone_forward(Map &map) const
{
    try
    {
        auto valid = this->validate_connection(map);
        if (!valid.success)
            throw std::logic_error(valid.why.value());
        Drone *_drone = this->hub1->drones.front();
        if (_drone == nullptr)
            throw std::logic_error("No drone to be moved");
        this->hub2->drones.push_back(_drone);
        this->hub1->drones.erase(this->hub1->drones.begin());
        return ExecuteState::Ok("Moving drone");
    }
    catch (std::exception &ex)
    {
        std::cout << "OH OH: " << ex.what() << endl;
        return ExecuteState::Fail(ex.what());
    }
}

ExecuteState Connection::drone_backward(Map &map) const
{
    try
    {
        auto valid = this->validate_connection(map);
        if (!valid.success)
            throw std::logic_error(valid.why.value());
        Drone *_drone = this->hub2->drones.front();
        if (_drone == nullptr)
            throw std::logic_error("No drone to be moved");
        this->hub1->drones.push_back(_drone);
        this->hub2->drones.erase(this->hub2->drones.begin());
        return ExecuteState::Ok("Moving drone");
    }
    catch (std::exception &ex)
    {
        std::cout << "OH OH: " << ex.what() << endl;
        return ExecuteState::Fail(ex.what());
    }
}
