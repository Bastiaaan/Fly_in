
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
            auto candidates = getConnectionCosts(*_hub);
            if (!candidates.empty())
            {
                std::cout << "Containing " << candidates.size() << " candidates" << std::endl;
                if (std::all_of(candidates.begin(), candidates.end(), [firstCost = get<0>(candidates.begin()->second)]
                    (std::pair<const std::string, std::tuple<float, Link *>> const &entry) -> bool
                    { return firstCost == std::get<0>(entry.second); }) && candidates.size() > 1)
                {
                    for (auto [name, link] : candidates)
                    {
                        Link *connection = get<1>(link);
                        _hub->transferDrone(*connection, hubRadius);
                        turns += connection->hub->zone == Restricted ? 2 : 1;
                    }
                }
                else
                {
                    auto cheapest = std::min_element(
                    candidates.begin(), candidates.end(),
                [](std::pair<const std::string, std::tuple<float, Link *>> const &a,
                    std::pair<const std::string, std::tuple<float, Link *>> const &b)
                    { return std::get<0>(a.second) < std::get<0>(b.second); });
                    auto value = *get<1>(cheapest->second);
                    _hub->transferDrone(value, hubRadius);
                    turns += value.hub->zone == Restricted ? 2 : 1;
                }
            }
        }
    }

    return turns;
}

bool Algorithm::foundTheEnd(Hub const &origin, Hub const &hub) {
    auto visited = new unordered_set<Hub const*>();
    bool const result = foundTheEnd(origin, hub, visited);
    delete visited;
    visited = nullptr;
    return result;
}

bool Algorithm::foundTheEnd(
    Hub const& origin,
    Hub const& hub,
    std::unordered_set<Hub const*>* visited)
{
    if (&hub == &origin)
        return false;

    // Circular trap
    if (!visited->insert(&hub).second)
        return false;

    // We reached an end
    if (hub.isEnd())
        return true;

    // No way forward
    if (hub.connections.empty())
        return false;

    // Search all possible outgoing connections
    for (auto const& [_, link] : hub.connections)
    {
        if (foundTheEnd(origin, *link->hub, visited))
            return true;
    }

    return false;
}

std::map<std::string, std::tuple<float, Link *>> Algorithm::getConnectionCosts(Hub const &hub)
{
    std::map<std::string, std::tuple<float, Link *>> costs;
    if (hub.connections.empty())
        return costs;
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
        if (!foundTheEnd(hub, *connection->hub))
             totalCost = INFINITY;
        if (totalCost != INFINITY)
            costs.insert({_, {totalCost / static_cast<float>(hub.drones.size()), connection}});
    }
    return costs;
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

unsigned int Algorithm::droneLimit(Hub const *hub, Link const &connection)
{
    unsigned int drones = hub->drones.size();
    unsigned int nextDrones = connection.hub->drones.size();

    if (connection.hub->max_drones.has_value() || connection.max_link_capacity.has_value())
    {
        if (connection.hub->max_drones.has_value())
        {
            if (nextDrones != 0)
            {
                if (nextDrones == connection.hub->max_drones.value())
                    return 0;
                if (nextDrones < connection.hub->max_drones.value())
                    return connection.hub->max_drones.value() - nextDrones;
            }
            else
            {
                return drones >= connection.hub->max_drones.value() ?
                     connection.hub->max_drones.value() : drones;
            }
        }
        if (connection.max_link_capacity.has_value())
            return drones >= connection.max_link_capacity.value() ?
            connection.max_link_capacity.value() : drones;
    }
    return drones;
}

bool Algorithm::readyFly(Link const &connection)
{
    return (connection.hub->drones.size() < connection.max_link_capacity &&
            connection.max_link_capacity.has_value()) ||
            (connection.hub->max_drones.has_value() && connection.hub->drones.size() < connection.hub->max_drones.value()) ||
            !connection.max_link_capacity.has_value() && !connection.hub->max_drones.has_value();
}