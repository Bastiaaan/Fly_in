#pragma once

#include "fly_in.hpp"
#include "hub.hpp"
#include "raylib.h"

class Hub;
class Drone {
	public:
		Vector2 *location;
		std::optional<Vector2*> destination;
		int id;
		Drone(int id);
		~Drone();
		int setDestination(Hub* destination, float hubRadius);
		bool flying() const;
		void moving();
};