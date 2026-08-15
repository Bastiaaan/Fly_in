
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

static int untilEnd(Hub const &hub, std::vector<int> &steps, int current = 0)
{
    if (hub.isEnd())
    {
        steps.push_back(current);
        return current;
    }

    if (hub.connections.empty())
        return static_cast<int>(INFINITY);

    for (auto const &connection : hub.connections)
    {
        Link *conn = connection.second;
        int penalty = (conn->hub->zone == Restricted) ? 1 : 0;
        untilEnd(*conn->hub, steps, current + 1 + penalty);
    }

    return steps.empty() ? static_cast<int>(INFINITY) : *std::min_element(steps.begin(), steps.end());
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
            int result = untilEnd(*_hub, steps, 0);
            auto candidates = getConnectionCosts(*_hub);
            if (!candidates.empty())
            {
                std::vector<Link*> equal_costs;
                std::copy_if(candidates.begin(), candidates.end(), std::back_inserter(equal_costs),
                    [](pair<Link *, int> rec1, pair<Link *, int> rec2){ return rec1.second == rec2.second; }, equal_costs);
                for (auto [link, cost] : candidates)
                {
                    _hub->transferDrone(*link, hubRadius);
                }
                // for (auto log : logs)
                //     std::cout << log->_output << " ", sys.logs.push_back(log);
                // std::cout << std::endl;
            }
        }
    }
    return 1;
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

std::map<Link *, int> Algorithm::getConnectionCosts(Hub const &hub)
{
    std::map<Link *, int> costs;
    if (hub.connections.empty())
        return costs;
    for (auto [_, connection] : hub.connections)
    {
        int cost = 0;
        if (connection->hub->zone == Restricted)
            cost += 4;
        else if (connection->hub->zone == Priority)
            cost += 1;
        else if (connection->hub->zone == Blocked)
            cost += 8;
        else
            cost += 2;

        if (connection->max_link_capacity > 1)
        {
            if (connection->max_link_capacity > hub.drones.size())
                cost += connection->max_link_capacity - hub.drones.size();
            else
                cost += connection->max_link_capacity;
        }
        if (connection->hub->max_drones > 1)
        {
            if (connection->hub->max_drones >= hub.drones.size())
                cost += connection->hub->max_drones - hub.drones.size();
            else
                cost += connection->hub->max_drones;
        }
        if (!foundTheEnd(hub, *connection->hub))
             cost += static_cast<int>(INFINITY);
        if (cost != static_cast<int>(INFINITY))
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
    unsigned int drones = hub->drones.size();
    unsigned int nextDrones = connection.hub->drones.size();

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
            connection.max_link_capacity == 1 && connection.hub->max_drones == 1;
}