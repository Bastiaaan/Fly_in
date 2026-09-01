//
// Created by brogaar on 29-6-26.
//

#include "fly_in.hpp"
#include "customExceptions.hpp"

Hub::Hub()
{
    this->location = nullptr;
    this->start = false;
    this->end = false;
}

Hub::~Hub()
{
    if (!this->drones.empty())
    {
        for (auto drone : this->drones) {
            delete drone;
            drone = nullptr;
        }
    }
    if (this->location != nullptr)
    {
        delete this->location;
        location = nullptr;
    }
	if (!this->connections.empty())
	{
		for(auto [name, link] : this->connections)
		{
			delete link;
			link = nullptr;
		}
		this->connections.clear();
	}
}

void Hub::setName(std::string const &name)
{
    this->name = name;
}

void Hub::setX(int const x)
{
    this->position_x = x;
}

void Hub::setY(int const y)
{
    this->position_y = y;
}

void Hub::setZone(string const &zoneName)
{
    if (zoneName == "restricted")
        this->zone = Zone::Restricted;
    else if (zoneName == "blocked")
        this->zone = Zone::Blocked;
    else if (zoneName ==  "priority")
        this->zone = Zone::Priority;
    else
        this->zone = Zone::Normal;
}

void Hub::setColor(std::string const &color)
{
    this->color = color;
}

void Hub::setMaxDrones(int maxDrones)
{
    this->max_drones = maxDrones;
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
            throw ParseException("Invalid startOrEnd", "1");
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

std::vector<Log*> Hub::transferDrone(Link &connection, float hubRadius)
{
    std::vector<Log*> logs;
    if (Algorithm::readyFly(this, connection) && !this->drones.empty())
    {
        unsigned int limit = Algorithm::droneLimit(this, connection);
		limit = std::min(limit, static_cast<unsigned int>(this->drones.size()));
        for (unsigned int _ = 0; _ < limit; _++)
        {
            Drone *drone = this->drones.front();
            drone->setDestination(connection.hub, hubRadius);
            this->drones.erase(this->drones.begin());
            connection.hub->drones.push_back(drone);
            auto _log = Log::output(*drone, *this, *connection.hub, limit, connection.max_link_capacity);
            logs.push_back(_log);
        }
    }
    return logs;
}
