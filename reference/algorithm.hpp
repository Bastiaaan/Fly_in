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
        static int calculateShortest(System &sys, Hub const *hub); // recursion may be applied here
        static bool correctSettled(System const &sys);
};