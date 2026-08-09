
#include "algorithm.hpp"

std::vector<Hub*> Algorithm::containingDrones(System const &sys)
{
    std::vector<Hub*> found;
    for (auto *hub : sys._map.hubs)
    {
        if (!hub->drones.empty())
            found.push_back(hub);
    }
    return found;
}

int Algorithm::rotateDrones(System const &sys, float const hubRadius)
{
    std::vector<Hub*> const found = containingDrones(sys);
    int turns = 0;
    if (found.empty())
        return turns;
    for (auto const *_hub : found)
    {
        if (_hub->connections.size() > 1)
        {
            std::map<std::string, int> costs;
            for (auto [name, connection] : _hub->connections)
            {
                int cost = connection.hub->zone == Restricted ? 2 : 1;
                

            }
        }
        else if (_hub->connections.size() == 1)
        {
            auto const connection = _hub->connections.begin();
            auto [hub, max_link_capacity] = connection->second;

            for (unsigned int i = 0; i < !max_link_capacity.has_value() ?
                _hub->drones.size() : max_link_capacity.value(); i++)
            {
                Drone *_drone = hub->drones[i];
                turns = _drone->setDestination(hub, hubRadius);
            }
        }
    }
    return turns;
}

void Algorithm::movingDrones(System const &sys)
{

}

bool Algorithm::correctSettled(System const &sys)
{
    return false;
}