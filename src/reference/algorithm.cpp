
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

int Algorithm::rotateDrones(System &sys, float const hubRadius)
{
    std::vector<Hub*> const found = containingDrones(sys);
    int turns = 0;
    if (found.empty())
        return turns;
    for (auto _hub : found)
    {
        if (!_hub->connections.empty())
        {
            std::vector<int> steps;
            auto candidates = getConnectionCosts(*_hub);
            if (!candidates.empty())
            {
                int lowestCost = std::min_element(candidates.begin(), candidates.end(),
                    [](std::pair<Link *, int> conn1, std::pair<Link *, int> conn2) -> bool
                    { return conn1.second < conn2.second; })->second;

                std::vector<pair<Link *, int>> selected;
                std::copy_if(candidates.begin(), candidates.end(), std::back_inserter(selected),
                    [lowestCost](const std::pair<Link*, int>& record) -> bool {
                    return record.second == lowestCost;
                });

                for (auto [link, cost] : selected)
                {
                    _hub->transferDrone(*link, hubRadius);
                }
                // for (auto log : logs)
                //     std::cout << log->_output << " ", sys.logs.push_back(log);
                // std::cout << std::endl;
            }
        }
    }
    return turns;
}

int Algorithm::untilTheEnd(Hub const &origin, Hub const &hub) {
    auto visited = new unordered_set<Hub const*>();
    bool const result = foundTheEnd(origin, hub, visited);
    int const steps = !result ? static_cast<int>(INFINITY) : visited->size();
    delete visited;
    visited = nullptr;
    return steps;
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

std::map<Link *, int> Algorithm::getConnectionCosts(Hub const &hub)
{
    std::map<Link *, int> costs;
    if (hub.connections.empty())
        return costs;
    for (auto [_, connection] : hub.connections)
    {
        int cost = 0;
        Hub const *nextHub = connection->hub;
        if (connection->hub->zone == Restricted)
            cost += 4;
        else if (connection->hub->zone == Priority)
            cost += 1;
        else if (connection->hub->zone == Blocked)
            cost += 8;
        else
            cost += 2;

        if (nextHub->max_drones == nextHub->drones.size())
            cost += 2;
        else
            cost += nextHub->drones.size() - nextHub->max_drones;
        if (untilTheEnd(hub, *connection->hub) != static_cast<int>(INFINITY))
            costs.insert({connection, cost});
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
}

unsigned int Algorithm::droneLimit(Hub const *hub, Link const &connection)
{
    unsigned int drones = static_cast<unsigned int>(hub->drones.size());
    unsigned int nextDrones = static_cast<unsigned int>(connection.hub->drones.size());

    if (connection.hub->max_drones > 1 || connection.max_link_capacity > 1)
    {
        if (connection.hub->max_drones > 1)
        {
            if (nextDrones != 0)
            {
                if (nextDrones == connection.hub->max_drones)
                    return 0;
                if (nextDrones < connection.hub->max_drones)
                    return connection.hub->max_drones - nextDrones;
            }
            else
            {
                return drones >= connection.hub->max_drones ?
                     connection.hub->max_drones : drones;
            }
        }
        if (connection.max_link_capacity > 1)
            return drones >= connection.max_link_capacity ?
            connection.max_link_capacity : drones;
    }
    return 1;
}

bool Algorithm::readyFly(Link const &connection)
{
    return (connection.hub->drones.size() < connection.max_link_capacity &&
            connection.max_link_capacity > 1) ||
            (connection.hub->max_drones > 1 && connection.hub->drones.size() < connection.hub->max_drones) ||
            (connection.max_link_capacity == 1 && connection.hub->max_drones == 1);
}