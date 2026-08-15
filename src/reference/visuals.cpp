
#include <cmath>
#include <set>

#include "visuals.hpp"

HubPoint* Renderer::locateHub(Hub &hub, std::vector<Line> const &lines)
{
    std::vector<Line> match;
    back_insert_iterator<vector<Line>> points = std::copy_if(lines.begin(), lines.end(), std::back_inserter(match),
                                                             [&hub](Line const &line) -> bool {
                                                                 return (hub.position_x == line.pos_x && hub.position_y == line.pos_y) ||
                                                                        (hub.position_x == -1 && hub.position_y == line.pos_y);
                                                             });

    if (match.size() == 2) {
        Line vert = match[0];
        Line hor = match[1];
        if (vert.type == "vertical" && hor.type == "horizontal")
        {
            while (vert.starty != hor.starty || hor.startx != vert.startx)
            {
                if (vert.starty != hor.starty) vert.starty++;
                if (hor.startx != vert.startx) hor.startx++;
            }
            return HubPoint::Save(hub, vert.startx, hor.starty);
        }
    }
    else if (match.size() == 1)
    {
        Line _line = match[0]; // this line is horizontal
        auto found = std::find_if(lines.begin(), lines.end(), [&hub](Line const &l) -> bool {
            return l.pos_x == hub.position_x &&
                   l.type == "vertical";
        });
        Line vert = *found; // this line is the vertical one. It was needed for localization of the hub
        while (_line.startx != vert.startx || _line.starty != vert.starty)
        {
            if (_line.startx != vert.startx) _line.startx++;
            if (_line.starty != vert.starty) vert.starty++;
        }
        return HubPoint::Save(hub, _line.startx, vert.starty);

    }
    return new HubPoint {hub, 0, 0};
}

Color Renderer::resolveColor(Hub &hub)
{
    if (hub.color == "default")
        return {128, 128, 128, 255};

    std::string const c = hub.color;

    if (c == "green")   return {0,   128, 0,   255};
    if (c == "red")     return {255, 0,   0,   255};
    if (c == "cyan")    return {0, 255, 255, 255};
    if (c == "purple")  return {128, 0,   128, 255};
    if (c == "black")   return {0,   0,   0,   255};
    if (c == "brown")   return {165, 42,  42,  255};
    if (c == "orange")  return {255, 165, 0,   255};
    if (c == "maroon")  return {128, 0,   0,   255};
    if (c == "gold")    return {255, 215, 0,   255};
    if (c == "darkred") return {139, 0,   0,   255};
    if (c == "violet")  return {238, 130, 238, 255};
    if (c == "crimson") return {220, 20,  60,  255};
    if (c == "yellow")  return {255, 255, 0,   255};
    if (c == "blue")    return {0,   0,   255, 255};
    if (c == "magenta") return {255, 0, 255, 255};
    if (c == "lime")    return {0, 255, 0, 255};
    return {128, 128, 128, 255};
}

// std::pair<std::pair<int, int>, std::pair<int, int>> Renderer::cutExcessPixels(Hub* origin, Link next)
// {
//     HubPoint *currentLoc = origin->location;
//     HubPoint *nextLoc = next.hub->location;
// }

void Renderer::renderBackground(System &sys, MeasureBank &sizes)
{
    ClearBackground(RAYWHITE);
    DrawRectangle(0, 0, sizes.screenWidth, sizes.startActionRadius_y, LIGHTGRAY);
    DrawRectangle(0, 0, sizes.startActionRadius_x, sizes.screenHeight, LIGHTGRAY);
    DrawRectangle(0, sizes.endActionRadius_y, sizes.screenWidth, sizes.startActionRadius_y, LIGHTGRAY);
    DrawRectangle(sizes.endActionRadius_x, 0, sizes.screenWidth, sizes.endActionRadius_x, LIGHTGRAY);
    DrawLine(sizes.endActionRadius_x, sizes.startActionRadius_y, sizes.endActionRadius_x, sizes.endActionRadius_y, DARKGRAY);
    DrawLine(sizes.startActionRadius_x, sizes.startActionRadius_y, sizes.endActionRadius_x, sizes.startActionRadius_y, DARKGRAY);
    DrawLine(sizes.startActionRadius_x, sizes.startActionRadius_y, sizes.startActionRadius_x, sizes.endActionRadius_y, DARKGRAY);
    DrawLine(sizes.startActionRadius_x, sizes.endActionRadius_y, sizes.endActionRadius_x, sizes.endActionRadius_y, DARKGRAY);
    DrawText(TextFormat("Map: %s", sys._map.name.c_str()), sizes.screenWidth / 3 + 230, 20, 50, BLACK);
}

void Renderer::renderBackgroundLines(System &sys, MeasureBank &sizes, map<string, bool> &checkList)
{
    unsigned long const total_x_radius = sizes.screenWidth;
    unsigned long const total_y_radius = sizes.screenHeight;
    unsigned long const step_y = total_y_radius / (sizes.hubRangeY.size() + 1);
    unsigned long incr_y = step_y;

    for (int ytem : sizes.hubRangeY)
    {
        unsigned long const step_x = total_x_radius / (sizes.hubRangeX.size() + 1);
        unsigned long incr_x = step_x;

        for (int xtem : sizes.hubRangeX)
        {
            auto vert_line = Line::Save(incr_x,        // x: start from 0 offset
                                        0,              // y: top of screen
                                        incr_x,         // x: same column
                                        sizes.screenHeight, // y: bottom of screen
                                      xtem,
                                      ytem,
                                 "vertical");
            DrawLine(vert_line.startx, vert_line.starty,
                             vert_line.endx,   vert_line.endy, LIGHTGRAY);
            if (!checkList["vertical_lines"])
                sizes.saveLine(vert_line);
            incr_x += step_x;
        }
        checkList["vertical_lines"] = true;
        auto horr_line = Line::Save(0,                  // x: left of screen
                                    incr_y,             // y: current row
                                    sizes.screenWidth,  // x: right of screen
                                    incr_y,             // y: same row
                                  -1,
                                  ytem,
                             "horizontal");
        DrawLine(horr_line.startx, horr_line.starty,
                         horr_line.endx,horr_line.endy, LIGHTGRAY);

        for (int i = 0; i <= sizes.hubRangeX.size(); i++)
        {
            horr_line.pos_x = i;
            if (!checkList["horizontal_lines"])
                sizes.saveLine(horr_line);
        }
        incr_y += step_y;
    }
    checkList["horizontal_lines"] = true;
}

void Renderer::renderHubs(System &sys, MeasureBank &sizes)
{
    for (auto const hub : sys._map.hubs)
    {
        auto hubLoc = Renderer::locateHub(*hub, sizes.lines);
        hub->location = hubLoc;

        float radius = 90;
        int fontSize = 25;
        if (sys._map.difficulty == "easy") radius = static_cast<float>(sizes.screenWidth / 31), fontSize = 50;
        if (sys._map.difficulty == "medium") radius = static_cast<float>(sizes.screenWidth / 34), fontSize = 40;
        if (sys._map.difficulty == "hard") radius = static_cast<float>(sizes.screenWidth / 55), fontSize = 20;
        if (sys._map.difficulty == "challenger") radius = static_cast<float>(sizes.screenWidth / 70), fontSize = 15;
        sizes.hubRadius.x = radius, sizes.hubRadius.y = radius;
        if (hubLoc->x > 0 && hubLoc->y > 0)
        {
            if (hub->zone == Blocked)
            {
                DrawPoly({static_cast<float>(hubLoc->x), static_cast<float>(hubLoc->y)},
                                  4, radius + 3, 0.0f, BLACK);
                DrawPoly({static_cast<float>(hubLoc->x), static_cast<float>(hubLoc->y)},
                                  4, radius, 0.0f, Renderer::resolveColor(*hub));
            }
            if (hub->zone == Restricted)
            {
                radius += 5;
                DrawPoly({static_cast<float>(hubLoc->x), static_cast<float>(hubLoc->y)},
                                  6, radius + 3, 0.0f, BLACK);
                DrawPoly({static_cast<float>(hubLoc->x), static_cast<float>(hubLoc->y)},
                                  6, radius, 0.0f, Renderer::resolveColor(*hub));
            }
            if (hub->zone == Priority)
            {
                radius += 10;
                DrawPoly({static_cast<float>(hubLoc->x), static_cast<float>(hubLoc->y)},
                                  3, radius + 3, 90.0f, BLACK);
                DrawPoly({static_cast<float>(hubLoc->x), static_cast<float>(hubLoc->y)},
                                  3, radius, 90.0f, Renderer::resolveColor(*hub));
            }
            if (hub->zone == Normal)
            {
                DrawCircle(hubLoc->x, hubLoc->y, radius + 3, BLACK);
                DrawCircle(hubLoc->x, hubLoc->y, radius, Renderer::resolveColor(*hub));
            }
            int textWidth = MeasureText(hub->name.c_str(), fontSize);
            DrawText(TextFormat("%s", hub->name.c_str()), hubLoc->x - (textWidth / 2), hubLoc->y + (radius + 5), fontSize, BLACK);
        }
    }
}

void Renderer::renderConnections(System &sys, MeasureBank const &sizes)
{
    for (auto *hub : sys._map.hubs)
    {
        if (hub->location == nullptr)
        {
            auto hubLoc = locateHub(*hub, sizes.lines);
            hub->location = hubLoc;
        }
    }

    for (auto const *hub : sys._map.hubs)
    {
        for (const auto &[fst, snd] : hub->connections)
        {
            auto const linked = snd->hub;
            Vector2 from = {static_cast<float>(hub->location->x), static_cast<float>(hub->location->y)};
            Vector2 to   = {static_cast<float>(linked->location->x), static_cast<float>(linked->location->y)};
            DrawLineEx(from, to, 5, GRAY);
        }
    }
}

void Renderer::renderDrones(System &sys, MeasureBank const &sizes)
{
    std::uniform_real_distribution angleDist(0.0f, 2.0f * static_cast<float>(M_PI));
    std::uniform_real_distribution<float> radiusDist(0.0f, sizes.hubRadius.x);

    for (auto hub : sys._map.hubs)
    {
        for (auto drone : hub->drones)
        {
            if (drone->location == nullptr)
            {
                float angle  = angleDist(rng);
                float radius = radiusDist(rng);
                drone->location = new Vector2 {
                    static_cast<float>(hub->location->x) + std::cos(angle) * radius,
                    static_cast<float>(hub->location->y) + std::sin(angle) * radius
                };
            }
            DrawPoly((*drone->location), 4, 33, 90.0f, BLACK);
            DrawPoly((*drone->location), 4, 30, 90.0f, GRAY);
            int measureTxt = MeasureText(TextFormat("#%d", drone->id), 15);
            DrawText(TextFormat("#%d", drone->id), drone->location->x - measureTxt, drone->location->y - measureTxt / 2, 20, BLACK);
        }
    }
}