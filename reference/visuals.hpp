
#pragma once
#include "fly_in.hpp"
#include <raylib.h>

struct Line
{
    int startx;
    int starty;
    int endx;
    int endy;
    static Line Save(int sx, int sy, int ex, int ey) {
        Line _new;
        _new.startx = sx;
        _new.starty = sy;
        _new.endx = ex;
        _new.endy = ey;
        return _new;
    }
};

struct MeasureBank
{
    int monitorID;
    int screenWidth;
    int screenHeight;
    int actionRadius_x;
    int actionRadius_y;
    int hubRangeX;
    int hubRangeY;
    std::vector<Line> lines;
    static MeasureBank Init(Map &map)
    {
        int const monitor = GetCurrentMonitor();
        MeasureBank bank;
        bank.monitorID = monitor;
        std::cout << "monitor_id is: " << monitor << std::endl;
        bank.screenWidth = (GetMonitorWidth(monitor) / 100) * 75;
        bank.screenHeight = (GetMonitorHeight(monitor) / 100) * 80;

        auto coordRange =  [map](char const c) -> int
        {
            if (c == 'x' || c == 'y')
            {
                int max = 0, min = 0;
                for (int x = 0; x < map.hubs.size(); x++)
                {
                    const int current = (c == 'x')
                        ? map.hubs[x]->position_x
                        : map.hubs[x]->position_y;
                    if (current > max)
                        max = current;
                    if (current < min)
                        min = current;
                }
                if (min < 0)
                    min *= -1;
                return (max - min) + 1;
            }
            return -1;
        };
        bank.actionRadius_x = bank.screenWidth - (bank.screenWidth / 15);
        bank.actionRadius_y = bank.screenHeight - (bank.screenHeight / 15);
        bank.hubRangeX = coordRange('x');
        bank.hubRangeY = coordRange('y');
        return bank;
    }
    void saveLine(Line const &line)
    {
        this->lines.push_back(line);
    }
};

struct HubPoint
{
    unsigned int x, y;
};