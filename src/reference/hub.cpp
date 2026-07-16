//
// Created by brogaar on 29-6-26.
//

#include "../../reference/fly_in.hpp"

Hub::Hub()
{

}

Hub::~Hub()
{

}

void Hub::setName(string name)
{
    this->name = name;
}

void Hub::setX(int x)
{
    this->position_x = x;
}

void Hub::setY(int y)
{
    this->position_y = y;
}

void Hub::setZone(optional<string> zoneName)
{
    if (zoneName == "restricted")
        this->zone = Zone::Restricted;
    if (zoneName == "blocked")
        this->zone = Zone::Blocked;
    if (zoneName ==  "priority")
        this->zone = Zone::Priority;
    else
        this->zone = Zone::Normal;
}

void Hub::setColor(optional<string> color)
{
    if (color != "")
        this->color = color;
    else
        this->color = "default";
}

void Hub::setMaxDrones(optional<int> max_drones)
{
    if (max_drones != nullopt)
        this->max_drones = max_drones;
}
