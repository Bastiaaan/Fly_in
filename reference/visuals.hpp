
#pragma once
#include "fly_in.hpp"
#include <raylib.h>

struct Line
{
    int pos_x = -1, pos_y = 0;
    int startx = 0;
    int starty = 0;
    int endx = 0;
    int endy = 0;
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
        bank.screenWidth = (GetMonitorWidth(monitor) / 100) * 90;
        bank.screenHeight = (GetMonitorHeight(monitor) / 100) * 85;
        auto _range = [&map](char const axis) -> std::vector<int>
		{
    		std::vector<int> result;

    		if ((axis != 'x' && axis != 'y') || map.hubs.empty())
        		return result;

    		int minCoord = axis == 'x' ? map.hubs.front()->position_x
                               : map.hubs.front()->position_y;
    		int maxCoord = minCoord;

    		for (auto const* hub : map.hubs)
    		{
    			if (hub == nullptr)
        			continue;

        		int const coord = axis == 'x' ? hub->position_x : hub->position_y;
        		minCoord = std::min(minCoord, coord);
        		maxCoord = std::max(maxCoord, coord);
    		}

    		for (int coord = minCoord; coord <= maxCoord; ++coord)
        		result.push_back(coord);

    		if (axis == 'y' && minCoord < 0)
        		std::reverse(result.begin(), result.end());

    		return result;
		};
        bank.startActionRadius_x = bank.screenWidth / 40;
        bank.startActionRadius_y = bank.screenHeight / 27;
        bank.endActionRadius_x = bank.screenWidth - bank.startActionRadius_x;
        bank.endActionRadius_y = bank.screenHeight - bank.startActionRadius_y;
        bank.hubRangeX = _range('x');
        bank.hubRangeY = _range('y');
		
		// ...existing code...
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
        static void renderBackgroundLines(MeasureBank &sizes, map<string, bool> &checkList);
        static void renderHubs(System &sys, MeasureBank &sizes);
        static void renderConnections(System &sys, MeasureBank const &sizes);
        static void renderDrones(System &sys, MeasureBank const &sizes);
};