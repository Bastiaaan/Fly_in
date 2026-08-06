
#pragma once
#include "fly_in.hpp"
#include "map.hpp"
#include "./hub.hpp"
#include <raylib.h>

struct Line
{
    int pos_x, pos_y;
    int startx;
    int starty;
    int endx;
    int endy;
    std::string type;
    static Line Save(int sx, int sy, int ex, int ey, int x = -1, int y = 0, std::string const &type = "") {
        auto result = Line();
        if (x != -1)
            result.pos_x = x;
        if (type == "horizontal" || type == "vertical")
            result.type = type;
        result.pos_y = y;
        result.startx = sx;
        result.starty = sy;
        result.endx = ex;
        result.endy = ey;
        return result;
    }
};

struct MeasureBank
{
    unsigned long monitorID;
    unsigned long screenWidth;
    unsigned long screenHeight;
    unsigned long startActionRadius_x;
    unsigned long startActionRadius_y;
    unsigned long endActionRadius_x;
    unsigned long endActionRadius_y;
    Vector2 hubRadius;
    std::vector<int> hubRangeX;
    std::vector<int> hubRangeY;
    std::vector<Line> lines;
    static MeasureBank Init(Map &map)
    {
        int const monitor = GetCurrentMonitor();
        MeasureBank bank;
        bank.monitorID = monitor;
        std::cout << "monitor_id is: " << monitor << std::endl;
        bank.screenWidth = (GetMonitorWidth(monitor) / 100) * 90;
        bank.screenHeight = (GetMonitorHeight(monitor) / 100) * 85;
        std::cout << "Screen width: " << bank.screenWidth << std::endl;
        std::cout << "Screen height: " << bank.screenHeight << std::endl;
        auto _range = [&map](char const c) -> std::vector<int>
        {
            std::vector<int> result;

            if (c == 'x' || c == 'y')
            {
                for (auto const *hub : map.hubs)
                {
                    int coord = c == 'x' ? hub->position_x : hub->position_y;
                    if (std::find(result.begin(), result.end(), coord) == result.end())
                        result.push_back(coord);
                }
                if (c == 'y' && std::any_of(result.begin(), result.end(), [](int const n){ return n < 0; }))
                    std::sort(result.begin(), result.end(), std::greater<int>{});
                else
                    std::sort(result.begin(), result.end());
            }
            return result;
        };
        bank.startActionRadius_x = bank.screenWidth / 40;
        bank.startActionRadius_y = bank.screenHeight / 27;
        bank.endActionRadius_x = bank.screenWidth - bank.startActionRadius_x;
        bank.endActionRadius_y = bank.screenHeight - bank.startActionRadius_y;
        bank.hubRangeX = _range('x');
        bank.hubRangeY = _range('y'); // each of them are incremented by two to get the right amount of lines for correct representation.
        return bank;
    }
    void saveLine(Line const &line)
    {
        this->lines.push_back(line);
    }
    void storeHubRadius(Vector2 const radius)
    {
        this->hubRadius = radius;
    }
};

class Renderer {
    public:
        static HubPoint* locateHub(Hub &hub, std::vector<Line> const &lines);
        static Color resolveColor(Hub &hub);
        static std::pair<std::pair<int, int>, std::pair<int, int>> cutExcessPixels(Hub *origin, Link next);
        static void renderBackground(System &sys, MeasureBank &sizes);
        static void renderBackgroundLines(System &sys, MeasureBank &sizes, map<string, bool> &checkList);
        static void renderHubs(System &sys, MeasureBank const &sizes);
        static void renderConnections(System &sys, MeasureBank const &sizes);
        static void renderDrones(System &sys, MeasureBank const &sizes);
};