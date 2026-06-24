
#ifndef DRONE_HPP
# define DRONE_HPP

#include "hub.hpp"

class Drone {
	public:
		int id;
		Drone(int id): id(id);
};

Drone::Drone(int id)
{
	this->id = id;
};

#endif