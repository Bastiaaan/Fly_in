#pragma once
#include "system.hpp"

// must behave like an independent member to make the drones move to the right hubs
// and to ensure of the shortest route and the least amount of turns.
// The so-called Dijkstra's algorithm is applied.
class Algorithm
{
    public:
        static int rotateDrones(System const &sys, float hubRadius);
        static std::vector<Hub*> containingDrones(System const &sys);
        static void movingDrones(System const &sys);
        static bool foundTheEnd(Hub const &hub); // recursion may be applied here
        static Link& cheapestPossible(Hub const &hub);
        static bool readyFly(Link const &connection);
        static unsigned int droneLimit(Hub const *hub, Link const *connection);
        static bool noDroneFlies(System const &sys);
};