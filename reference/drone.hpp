
#ifndef DRONE_HPP
# define DRONE_HPP

#include "fly_in.hpp"
#pragma once

class Drone {
	public:
		int id;
		Drone(int id);
		~Drone() = default;
};

#endif