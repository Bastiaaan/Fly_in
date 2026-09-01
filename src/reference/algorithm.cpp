
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
	std::vector<int> rotations;
	std::vector<Log*> logs;
    std::vector<Hub*> const found = containingDrones(sys);
    int turn = 0;
	
    if (found.empty())
        return turn;
    for (auto _hub : found)
    {
        if (!_hub->connections.empty())
        {
            std::vector<int> steps;
            auto candidates = getConnectionCosts(*_hub);
            if (!candidates.empty())
            {
                int lowestCost =
				get<0>(std::min_element(candidates.begin(), candidates.end(),
                    [](std::pair<Link *, std::tuple<int, int>> conn1,
					   std::pair<Link *, std::tuple<int, int>> conn2) -> bool
                    { return get<0>(conn1.second) < get<0>(conn2.second); })->second);
				
				int leastStep =
				get<1>(std::min_element(candidates.begin(), candidates.end(),
					[](std::pair<Link *, std::tuple<int, int>> conn1,
					   std::pair<Link *, std::tuple<int, int>> conn2) -> bool
					{ return get<1>(conn1.second) < get<1>(conn2.second); })->second);

                std::vector<pair<Link *, std::tuple<int, int>>> selected;
                std::copy_if(candidates.begin(), candidates.end(),
				std::back_inserter(selected),
                    [lowestCost](const std::pair<Link *, std::tuple<int, int>>& record) -> bool {
                    return get<0>(record.second) == lowestCost;
                });
				turn = std::any_of(selected.begin(), selected.end(),
					[](std::pair<Link *, std::tuple<int, int>> &row) -> bool {
						return row.first->hub->zone == Restricted; 
					}) ? 2 : 1;

				if (std::all_of(selected.begin(), selected.end(),
					[lowestCost](std::pair<Link *, std::tuple<int, int>> const &row) -> bool {
						return get<0>(row.second) == lowestCost;
					}) && _hub->drones.size() == 1) // this should only apply if the current zone has only one drone
				{
					selected.erase(std::remove_if(selected.begin(), selected.end(),
						[leastStep](std::pair<Link *, std::tuple<int, int>> const &row) -> bool {
							return get<1>(row.second) != leastStep;
						}), selected.end());
				}
                for (auto [link, cost] : selected)
                {
                    auto _logs = _hub->transferDrone(*link, hubRadius);
					for (auto &log : _logs)
						logs.push_back(log);
                }
				rotations.push_back(turn);
            }
			else
				rotations.push_back(0);
        }
    }

	if (std::all_of(rotations.begin(), rotations.end(),
		[](int const &rot) -> bool { return rot == 0; }))
		return 0;

	sys.logs[sys.logs.size() + 1] = logs;
    return std::any_of(rotations.begin(), rotations.end(),
		[](int &rec) -> bool { return rec == 2; }) ? 2 : 1;
}

int Algorithm::untilTheEnd(Hub const &origin, Hub const &hub)
{
    auto visited = new std::unordered_set<Hub const*>();
    bool const result = foundTheEnd(origin, hub, visited);
    int const steps = !result ? 2147483647 : visited->size();

    if (steps == 2147483647)
		return steps;

	if (std::any_of(visited->begin(),
		visited->end(), [](Hub const *_h) -> bool {
		return _h->zone == Blocked; }))
		return 2147483647;

    auto totalCost = [&visited]() -> int
    {
        int cost = 0;
        for (auto hub = visited->begin(); hub != visited->end(); hub++)
        {
            cost += (*hub)->zone != Restricted ? 1 : 2;
        }
        return cost;
    };

    int total = totalCost();
    delete visited;
    visited = nullptr;
    return total;
}

bool Algorithm::foundTheEnd(
    Hub const& origin,
    Hub const& hub,
    std::unordered_set<Hub const*>* visited)
{
    if (&hub == &origin)
        return false;

    if (!visited->insert(&hub).second)
        return false;

    if (hub.isEnd())
        return true;

    if (hub.connections.empty())
        return false;

    for (auto const& [_, link] : hub.connections)
    {
        if (foundTheEnd(origin, *link->hub, visited))
            return true;
    }

    return false;
}

std::map<Link *, std::tuple<int, int>> Algorithm::getConnectionCosts(Hub const &hub)
{
    std::map<Link *, std::tuple<int, int>> costs;
    if (hub.connections.empty())
        return costs;
    for (auto [_, connection] : hub.connections)
    {
        int cost = 0;
        Hub const *nextHub = connection->hub;
		int const space = droneLimit(&hub, *connection);
		int const steps = untilTheEnd(hub, *nextHub); 
        if (connection->hub->zone == Restricted)
            cost += 6;
        else if (connection->hub->zone == Priority)
            cost += 0;
        else
            cost += 3;
		if (space < 1)
			cost += 5;
        else
            cost += 2;
        if (nextHub->max_drones == nextHub->drones.size())
            cost += 5;
        else
            cost -= nextHub->max_drones;
		if (cost < 0)
			cost = 0;
        if (steps != 2147483647)
            costs.insert({connection, {cost, steps}});
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
	unsigned int hubSpace = connection.hub->max_drones - nextDrones;

	if (nextDrones >= connection.hub->max_drones)
		return 0;
	unsigned int limit = std::min({drones, hubSpace, connection.max_link_capacity});
	return std::max(limit, 0u);
}

bool Algorithm::readyFly(Hub const *hub, Link const &connection)
{
    if (hub == nullptr || connection.hub == nullptr || hub->drones.empty())
        return false;

    bool const hasHubCapacity =
        connection.hub->drones.size() < connection.hub->max_drones;

    bool const hasLinkCapacity =
        connection.max_link_capacity >= 1 &&
        connection.hub->drones.size() < connection.max_link_capacity;

    return hasHubCapacity || hasLinkCapacity;
}
