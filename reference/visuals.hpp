
#pragma once
#include "fly_in.hpp"

template <class C>
struct Canvas
{
    C type;
    unsigned int width, height;
};

struct HubPoint
{
    unsigned int x, y;
};

struct Line
{
    unsigned int start_x, end_x, start_y, end_y;
    static HubPoint markHubPoint(Line &line1, Line &line2)
    {

    }
};

class Visualizer {
    unsigned int winWidth, winHeight;
    public:
        Visualizer(unsigned int const winWidth, unsigned int const winHeight);
        template <class C>
        static Canvas<C> render_canvas()
        {
            return new Canvas<C>(100, 100);
        }
};