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
        static int searchForEnd(System &sys, Hub const *hub); // recursion may be applied here
        static Link& cheapest(std::map<std::string, Link*> connections);
        static bool correctSettled(System const &sys);
        static bool noDroneFlies(System const &sys);
};