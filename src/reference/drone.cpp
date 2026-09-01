//
// Created by brogaar on 29-6-26.
//

#include "fly_in.hpp"

Drone::Drone(int const id)
{
    this->id = id;
    this->location = nullptr;
}

Drone::~Drone()
{
    if (this->location != nullptr)
    {
        delete this->location;
        this->location = nullptr;
    }
}

int Drone::setDestination(Hub* const destination, float hubRadius)
{
    std::uniform_real_distribution angleDist(0.0f, 2.0f * static_cast<float>(M_PI));
    std::uniform_real_distribution<float> radiusDist(0.0f, hubRadius);
    float angle  = angleDist(rng);
    float radius = radiusDist(rng);
    this->destination = new Vector2 {
        static_cast<float>(destination->location->x) + std::cos(angle) * radius,
          static_cast<float>(destination->location->y) + std::sin(angle) * radius
    };
    if (destination->zone == Restricted)
        return 2;
    return 1;
}

bool Drone::flying() const
{
    if (!this->destination.has_value())
        return false;
    return this->location->x != this->destination.value()->x &&
           this->location->y != this->destination.value()->y;
}

void Drone::moving()
{
    if (this->flying())
    {
        float speed = 475.0f;

        float dx = this->destination.value()->x - this->location->x;
        float dy = this->destination.value()->y - this->location->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 1.0f)
        {
            delete this->destination.value();
            this->destination.reset();
        }
        else
        {
            float step = std::min(speed * GetFrameTime(), distance);
            float dirX = dx / distance;
            float dirY = dy / distance;

            this->location->x += dirX * step;
            this->location->y += dirY * step;
        }
    }
}
