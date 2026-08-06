//
// Created by brogaar on 29-6-26.
//

#include "fly_in.hpp"
#include "customExceptions.hpp"

Hub::~Hub()
{
    if (!this->drones.empty())
    {
        for (auto const drone : this->drones)
            delete[] drone;
    }
}

void Hub::setName(std::string const &name)
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

void Hub::setZone(optional<string> const &zoneName)
{
    if (zoneName.has_value())
    {
        std::cout << "Registered zone: " << zoneName.value() << std::endl;
        if (zoneName.value() == "restricted")
            this->zone = Zone::Restricted;
        else if (zoneName.value() == "blocked")
            this->zone = Zone::Blocked;
        else if (zoneName.value() ==  "priority")
            this->zone = Zone::Priority;
        else
            this->zone = Zone::Normal;
        std::cout << "this zone has id: " << this->zone.value() << std::endl;
    }
}

void Hub::setColor(optional<string> const &color)
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

void Hub::setStartOrEnd(std::string const &soe)
{
    try
    {
        if (soe == "start")
            this->start = true;
        else if (soe == "end")
            this->end = true;
        else
            throw ParseException("Invalid startOrEnd");
    }
    catch (ParseException &e) {
        std::cout << e.what() << std::endl;
    }

}

bool Hub::isStart() const
{
    return this->start != false;
}
bool Hub::isEnd() const
{
    return this->end != false;
}

void Hub::dropInfo() const
{
    std::cout << "Information about Hub #" << this->name << ':' << endl << endl;
    std::cout << " -\tX: " << this->position_x << endl;
    std::cout << " -\tY: " << this->position_y << endl;
    if (this->color.has_value())
        std::cout << " -\tColor: " << this->color.value() << endl;
    if (this->zone.has_value())
        std::cout << " -\tZone: " << this->zone.value() << endl;
    if (this->max_drones.has_value())
        std::cout << "- \tMax drones: " << this->max_drones.value() << endl;
    std::cout << endl;
}
