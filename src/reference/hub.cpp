//
// Created by brogaar on 29-6-26.
//

#include "../../reference/fly_in.hpp"

Hub::Hub(string const &name,
         int x,
         int y,
         optional<string> &zone,
         optional<string> &color,
         optional<int> max_drones)
{
    this->name = name;
    this->position_x = x;
    this->position_y = y;
    if (zone != "")
    {
        if (zone == "restricted")
            this->zone = Zone::Restricted;
        else if (zone == "blocked")
            this->zone = Zone::Blocked;
        else if (zone == "priority")
            this->zone = Zone::Priority;
    }
    else
        this->zone = Zone::Normal;
    if (color != "")
        this->color = color;
    else
        this->color = "default";
    if (max_drones > 0)
        this->max_drones = max_drones;
    else
        this->max_drones = nullopt;
}

Hub::~Hub()
{

}

HubData::HubData()
{
    this->limit = 6;
    this->index = 0;
}