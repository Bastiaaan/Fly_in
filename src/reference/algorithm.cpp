
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

bool Algorithm::noDroneFlies(System const &sys)
{
    return std::all_of(
        sys._map.hubs.begin(),
        sys._map.hubs.end(),
        [](Hub const *h) -> bool {
            return std::none_of(h->drones.begin(), h->drones.end(),
                [](Drone const *d) -> bool { return d->flying(); });
        }
    );
}

int Algorithm::rotateDrones(System const &sys, float const hubRadius)
{
    std::vector<Hub*> const found = containingDrones(sys);
    int turns = 0;
    if (found.empty())
        return turns;
    for (auto *_hub : found)
    {
        if (_hub->connections.size() > 0)
        {
            std::map<std::string, int> costs;
            for (auto [name, connection] : _hub->connections)
            {
                for (int index = 0;
                         index < (!connection->max_link_capacity.has_value()
                             ? _hub->drones.size()
                             : connection->max_link_capacity.value()); index++)
                {
                    if (connection->hub->drones.size() < connection->max_link_capacity)
                    {
                        Drone *first = _hub->drones.front();
                        first->setDestination(connection->hub, hubRadius);
                        _hub->drones.erase(_hub->drones.begin());
                        connection->hub->drones.push_back(first);
                    }
                }
            }
        }
    }
    return turns;
}

int Algorithm::searchForEnd(System &sys, Hub const *hub)
// returning the necessary turns to take to reach that specific end and re-calculating the factors.
{
    int turns = 0;
    if (hub->connections.size() == 1)
    {
        auto [_hub, link] = *hub->connections.begin()->second;
        turns += searchForEnd(sys, _hub);
    }
    else if (hub->connections.size() > 1)
    {

    }
    else
        return turns;
}

Link& Algorithm::cheapest(std::map<std::string, Link*> connections)
{
    std::map<std::string, float> costs;
    for (auto [_, connection] : connections)
    {

    }
}

void Algorithm::movingDrones(System const &sys)
{
    std::vector<Hub*> const withDrones = containingDrones(sys);
    for (auto *hub : withDrones)
    {
        for (auto *drone : hub->drones)
            drone->moving();
    }
}

bool Algorithm::correctSettled(System const &sys)
{
    return false;
}