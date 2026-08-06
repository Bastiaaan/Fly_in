#pragma once

#include "raylib.h"

class Drone {
	public:
		Vector2 *location;
		int id;
		Drone(int id);
		~Drone();
};