//
// Created by brogaar on 29-6-26.
//

#include "../../reference/fly_in.hpp"

Drone::Drone(int const id)
{
    this->id = id;
    this->location = nullptr;
}

Drone::~Drone()
{
    if (this->location != nullptr)
        delete this->location;
}

int Drone::setDestination(Hub* destination, float hubRadius)
{
    std::uniform_real_distribution angleDist(0.0f, 2.0f * static_cast<float>(M_PI));
    std::uniform_real_distribution<float> radiusDist(0.0f, hubRadius);
    float angle  = angleDist(rng);
    float radius = radiusDist(rng);
    this->destination = new Vector2 {
        static_cast<float>(destination->location->x) + std::cos(angle) * radius,
          static_cast<float>(destination->location->y) + std::sin(angle) * radius
    };
    if (destination->zone.value() == Restricted)
        return 2;
    return 1;
}

bool Drone::flying() const {
    if (!this->destination.has_value())
        return false;
    return this->location->x == this->destination.value()->x &&
           this->location->y == this->destination.value()->y;
}
