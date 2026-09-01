
#include <cmath>
#include <set>
#include "visuals.hpp"
#include "raylib.h"

HubPoint* Renderer::locateHub(Hub &hub, std::vector<Line> const &lines)
{
    std::vector<Line> match;
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(match),
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
                   l.type == "vertical"; });
        Line vert = *found; 
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

    static const std::map<std::string, Color> css = {
        {"aliceblue",            {240, 248, 255, 255}},
        {"antiquewhite",         {250, 235, 215, 255}},
        {"aqua",                 {0,   255, 255, 255}},
        {"aquamarine",           {127, 255, 212, 255}},
        {"azure",                {240, 255, 255, 255}},
        {"beige",                {245, 245, 220, 255}},
        {"bisque",               {255, 228, 196, 255}},
        {"black",                {0,   0,   0,   255}},
        {"blanchedalmond",       {255, 235, 205, 255}},
        {"blue",                 {0,   0,   255, 255}},
        {"blueviolet",           {138, 43,  226, 255}},
        {"brown",                {165, 42,  42,  255}},
        {"burlywood",            {222, 184, 135, 255}},
        {"cadetblue",            {95,  158, 160, 255}},
        {"chartreuse",           {127, 255, 0,   255}},
        {"chocolate",            {210, 105, 30,  255}},
        {"coral",                {255, 127, 80,  255}},
        {"cornflowerblue",       {100, 149, 237, 255}},
        {"cornsilk",             {255, 248, 220, 255}},
        {"crimson",              {220, 20,  60,  255}},
        {"cyan",                 {0,   255, 255, 255}},
        {"darkblue",             {0,   0,   139, 255}},
        {"darkcyan",             {0,   139, 139, 255}},
        {"darkgoldenrod",        {184, 134, 11,  255}},
        {"darkgray",             {169, 169, 169, 255}},
        {"darkgreen",            {0,   100, 0,   255}},
        {"darkgrey",             {169, 169, 169, 255}},
        {"darkkhaki",            {189, 183, 107, 255}},
        {"darkmagenta",          {139, 0,   139, 255}},
        {"darkolivegreen",       {85,  107, 47,  255}},
        {"darkorange",           {255, 140, 0,   255}},
        {"darkorchid",           {153, 50,  204, 255}},
        {"darkred",              {139, 0,   0,   255}},
        {"darksalmon",           {233, 150, 122, 255}},
        {"darkseagreen",         {143, 188, 143, 255}},
        {"darkslateblue",        {72,  61,  139, 255}},
        {"darkslategray",        {47,  79,  79,  255}},
        {"darkslategrey",        {47,  79,  79,  255}},
        {"darkturquoise",        {0,   206, 209, 255}},
        {"darkviolet",           {148, 0,   211, 255}},
        {"deeppink",             {255, 20,  147, 255}},
        {"deepskyblue",          {0,   191, 255, 255}},
        {"dimgray",              {105, 105, 105, 255}},
        {"dimgrey",              {105, 105, 105, 255}},
        {"dodgerblue",           {30,  144, 255, 255}},
        {"firebrick",            {178, 34,  34,  255}},
        {"floralwhite",          {255, 250, 240, 255}},
        {"forestgreen",          {34,  139, 34,  255}},
        {"fuchsia",              {255, 0,   255, 255}},
        {"gainsboro",            {220, 220, 220, 255}},
        {"ghostwhite",           {248, 248, 255, 255}},
        {"gold",                 {255, 215, 0,   255}},
        {"goldenrod",            {218, 165, 32,  255}},
        {"gray",                 {128, 128, 128, 255}},
        {"green",                {0,   128, 0,   255}},
        {"greenyellow",          {173, 255, 47,  255}},
        {"grey",                 {128, 128, 128, 255}},
        {"honeydew",             {240, 255, 240, 255}},
        {"hotpink",              {255, 105, 180, 255}},
        {"indianred",            {205, 92,  92,  255}},
        {"indigo",               {75,  0,   130, 255}},
        {"ivory",                {255, 255, 240, 255}},
        {"khaki",                {240, 230, 140, 255}},
        {"lavender",             {230, 230, 250, 255}},
        {"lavenderblush",        {255, 240, 245, 255}},
        {"lawngreen",            {124, 252, 0,   255}},
        {"lemonchiffon",         {255, 250, 205, 255}},
        {"lightblue",            {173, 216, 230, 255}},
        {"lightcoral",           {240, 128, 128, 255}},
        {"lightcyan",            {224, 255, 255, 255}},
        {"lightgoldenrodyellow", {250, 250, 210, 255}},
        {"lightgray",            {211, 211, 211, 255}},
        {"lightgreen",           {144, 238, 144, 255}},
        {"lightgrey",            {211, 211, 211, 255}},
        {"lightpink",            {255, 182, 193, 255}},
        {"lightsalmon",          {255, 160, 122, 255}},
        {"lightseagreen",        {32,  178, 170, 255}},
        {"lightskyblue",         {135, 206, 250, 255}},
        {"lightslategray",       {119, 136, 153, 255}},
        {"lightslategrey",       {119, 136, 153, 255}},
        {"lightsteelblue",       {176, 196, 222, 255}},
        {"lightyellow",          {255, 255, 224, 255}},
        {"lime",                 {0,   255, 0,   255}},
        {"limegreen",            {50,  205, 50,  255}},
        {"linen",                {250, 240, 230, 255}},
        {"magenta",              {255, 0,   255, 255}},
        {"maroon",               {128, 0,   0,   255}},
        {"mediumaquamarine",     {102, 205, 170, 255}},
        {"mediumblue",           {0,   0,   205, 255}},
        {"mediumorchid",         {186, 85,  211, 255}},
        {"mediumpurple",         {147, 112, 219, 255}},
        {"mediumseagreen",       {60,  179, 113, 255}},
        {"mediumslateblue",      {123, 104, 238, 255}},
        {"mediumspringgreen",    {0,   250, 154, 255}},
        {"mediumturquoise",      {72,  209, 204, 255}},
        {"mediumvioletred",      {199, 21,  133, 255}},
        {"midnightblue",         {25,  25,  112, 255}},
        {"mintcream",            {245, 255, 250, 255}},
        {"mistyrose",            {255, 228, 225, 255}},
        {"moccasin",             {255, 228, 181, 255}},
        {"navajowhite",          {255, 222, 173, 255}},
        {"navy",                 {0,   0,   128, 255}},
        {"oldlace",              {253, 245, 230, 255}},
        {"olive",                {128, 128, 0,   255}},
        {"olivedrab",            {107, 142, 35,  255}},
        {"orange",               {255, 165, 0,   255}},
        {"orangered",            {255, 69,  0,   255}},
        {"orchid",               {218, 112, 214, 255}},
        {"palegoldenrod",        {238, 232, 170, 255}},
        {"palegreen",            {152, 251, 152, 255}},
        {"paleturquoise",        {175, 238, 238, 255}},
        {"palevioletred",        {219, 112, 147, 255}},
        {"papayawhip",           {255, 239, 213, 255}},
        {"peachpuff",            {255, 218, 185, 255}},
        {"peru",                 {205, 133, 63,  255}},
        {"pink",                 {255, 192, 203, 255}},
        {"plum",                 {221, 160, 221, 255}},
        {"powderblue",           {176, 224, 230, 255}},
        {"purple",               {128, 0,   128, 255}},
        {"red",                  {255, 0,   0,   255}},
        {"rosybrown",            {188, 143, 143, 255}},
        {"royalblue",            {65,  105, 225, 255}},
        {"saddlebrown",          {139, 69,  19,  255}},
        {"salmon",               {250, 128, 114, 255}},
        {"sandybrown",           {244, 164, 96,  255}},
        {"seagreen",             {46,  139, 87,  255}},
        {"seashell",             {255, 245, 238, 255}},
        {"sienna",               {160, 82,  45,  255}},
        {"silver",               {192, 192, 192, 255}},
        {"skyblue",              {135, 206, 235, 255}},
        {"slateblue",            {106, 90,  205, 255}},
        {"slategray",            {112, 128, 144, 255}},
        {"slategrey",            {112, 128, 144, 255}},
        {"snow",                 {255, 250, 250, 255}},
        {"springgreen",          {0,   255, 127, 255}},
        {"steelblue",            {70,  130, 180, 255}},
        {"tan",                  {210, 180, 140, 255}},
        {"teal",                 {0,   128, 128, 255}},
        {"thistle",              {216, 191, 216, 255}},
        {"tomato",               {255, 99,  71,  255}},
        {"turquoise",            {64,  224, 208, 255}},
        {"violet",               {238, 130, 238, 255}},
        {"wheat",                {245, 222, 179, 255}},
        {"white",                {255, 255, 255, 255}},
        {"whitesmoke",           {245, 245, 245, 255}},
        {"yellow",               {255, 255, 0,   255}},
        {"yellowgreen",          {154, 205, 50,  255}},
        {"rainbow",              {128, 128, 128, 255}}, // handled separately if needed
    };

    auto it = css.find(hub.color);
    if (it != css.end())
        return it->second;

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
	DrawText(TextFormat("Turn: %d", sys.turn), sizes.endActionRadius_x - 220, sizes.startActionRadius_y + 40, 50, BLACK);
}

void Renderer::renderBackgroundLines(MeasureBank &sizes, map<string, bool> &checkList)
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
            auto vert_line = Line::Save(incr_x,
                                        0,
                                        incr_x,
                                        sizes.screenHeight,
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
        auto horr_line = Line::Save(0,
                                    incr_y,
                                    sizes.screenWidth,
                                    incr_y,
                                  -1,
                                  ytem,
                             "horizontal");
        DrawLine(horr_line.startx, horr_line.starty,
                         horr_line.endx,horr_line.endy, LIGHTGRAY);

        for (unsigned int i = 0; i <= sizes.hubRangeX.size(); i++)
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

        float radius = (sizes.screenWidth / 40);
        int fontSize = 25;
        if (sys._map.difficulty == "easy") radius = (sizes.screenWidth / 25), fontSize = 50;
        if (sys._map.difficulty == "medium") radius = (sizes.screenWidth / 34), fontSize = 40;
        if (sys._map.difficulty == "hard") radius = (sizes.screenWidth / 48), fontSize = 20;
        if (sys._map.difficulty == "challenger") radius = (sizes.screenWidth / 59), fontSize = 15;
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

    Color const linkColor = { 120, 120, 120, 255 };

    for (auto const *hub : sys._map.hubs)
    {
        for (const auto &[fst, snd] : hub->connections)
        {
            auto const linked = snd->hub;
            if (hub->location == nullptr || linked->location == nullptr)
                continue;

            Vector2 from = { static_cast<float>(hub->location->x), static_cast<float>(hub->location->y) };
            Vector2 to   = { static_cast<float>(linked->location->x), static_cast<float>(linked->location->y) };

            Vector2 delta = { to.x - from.x, to.y - from.y };
            float len = std::hypot(delta.x, delta.y);
            if (len < 1.0f)
                continue;

            Vector2 dir = { delta.x / len, delta.y / len };
            Vector2 perp = { -dir.y, dir.x };

            float radius = sizes.hubRadius.x;

            Vector2 start = {
                from.x + dir.x * (radius + 6.0f),
                from.y + dir.y * (radius + 6.0f)
            };

            Vector2 end = {
                to.x - dir.x * (radius + 10.0f),
                to.y - dir.y * (radius + 10.0f)
            };

            DrawLineEx(start, end, 5, linkColor);

            float headLength = 30.0f;
            float headHalf = 16.0f;

            Vector2 tip = end;
            Vector2 base = {
                end.x - dir.x * headLength,
                end.y - dir.y * headLength
            };

            Vector2 left = {
                base.x + perp.x * headHalf,
                base.y + perp.y * headHalf
            };

            Vector2 right = {
                base.x - perp.x * headHalf,
                base.y - perp.y * headHalf
            };

            DrawTriangle(tip, right, left, linkColor);
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