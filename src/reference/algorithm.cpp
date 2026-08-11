
#include "algorithm.hpp"

std::vector<Hub*> Algorithm::containingDrones(System const &sys)
{
    std::vector<Hub*> found;
    for (auto const hub : sys._map.hubs)
    {
        if (!hub->drones.empty())
            found.insert(found.begin(), hub);
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
    for (auto &_hub : found)
    {
        if (!_hub->connections.empty())
        {
            auto connection = _hub->connections.size() > 1 ? cheapestPossible(*_hub) : *_hub->connections.begin()->second;
            if (readyFly(connection))
            {
                unsigned int const limit = droneLimit(_hub, connection);
                for (int _ = 0; _ < limit; _++)
                {
                    Drone *drone = _hub->drones.front();
                    drone->setDestination(connection->hub, hubRadius);
                    _hub->drones.erase(_hub->drones.begin());
                    connection->hub->drones.push_back(drone);
                }
            }
        }
    }

    return turns;
}

bool Algorithm::foundTheEnd(Hub const &hub)
// returning the necessary turns to take to reach that specific end and re-calculating the factors.
{
    bool search = false;
    if (hub.connections.size() == 1)
    {
        auto [_hub, link] = *hub.connections.begin()->second;
        search = foundTheEnd(*_hub);
    }
    else if (hub.connections.size() > 1)
    {
        // todo: Searching each option until there's an end.
        for (auto [hub, link] : hub.connections)
        {
            search = foundTheEnd(*link->hub);
        }
    }
    else
    {
        if (hub.isEnd())
            search = true;
        else
            search = false;
    }
    return search;
}

Link& Algorithm::cheapestPossible(Hub const &hub)
{
    std::map<std::string, float> costs;
    for (auto [_, connection] : hub.connections)
    {
        float totalCost = 0.0f;
        if (connection->hub->zone == Restricted)
            totalCost += 5.5f;
        else if (connection->hub->zone == Priority)
            totalCost += 0.33f;
        else if (connection->hub->zone == Blocked)
            totalCost += 10.0f;
        else
            totalCost += 1.5f;

        if (connection->max_link_capacity.has_value())
        {
            if (connection->max_link_capacity.value() > hub.drones.size())
                totalCost += static_cast<float>
                             (connection->max_link_capacity.value()) -
                             static_cast<float>(hub.drones.size());
            else
                totalCost += static_cast<float>(hub.drones.size());
        }
        if (connection->hub->max_drones.has_value())
        {
            if (connection->hub->max_drones.value() >= hub.drones.size())
                totalCost += static_cast<float>(connection->hub->max_drones.value())
                    / static_cast<float>(hub.drones.size());
            else
                totalCost += static_cast<float>(hub.drones.size());
        }
        if (!foundTheEnd(*connection->hub))
            totalCost += 12.5;
        costs.insert({_, totalCost / static_cast<float>(hub.drones.size())});
    }
    auto result = std::min_element(
        costs.begin(),
        costs.end(),
        [](auto const &a, auto const &b) { return a.second < b.second; });
    if (result != costs.end())
    {
        std::string const& key = result->first;
        float value = result->second;
    }

    return *hub.connections.find(result->first)->second;
}

void Algorithm::movingDrones(System const &sys)
{
    std::vector<Hub*> const withDrones = containingDrones(sys);
    for (auto *hub : withDrones)
    {
        for (auto *drone : hub->drones)
            drone->moving();
    }
    std::cout << std::endl;
}

unsigned int Algorithm::droneLimit(Hub const &hub, Link const &connection)
{
    if (connection.hub->max_drones.has_value() || connection.max_link_capacity.has_value())
    {
        if (connection.hub->max_drones.has_value()) return connection.hub->max_drones.value();
        if (connection.max_link_capacity.has_value()) return connection.max_link_capacity.value();
    }
    return hub.drones.size();
}

bool Algorithm::readyFly(Link const &connection)
{
    return (connection.hub->drones.size() < connection.max_link_capacity &&
            connection.max_link_capacity.has_value()) ||
            (connection.hub->max_drones.has_value() && connection.hub->drones.size() < connection.hub->max_drones.value()) ||
            !connection.max_link_capacity.has_value() && !connection.hub->max_drones.has_value();
}