

#include "visuals.hpp"

HubPoint Visualizer::locateHub(Hub &hub, std::vector<Line> const &lines)
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
            while (vert.starty != hor.starty && hor.startx != vert.startx)
                vert.starty++, hor.startx++;
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
    return {hub, 0, 0};
}

Color Visualizer::resolveColor(Hub &hub) {
    if (!hub.color.has_value())
        return {128, 128, 128, 255};

    std::string const c = hub.color.value();

    if      (c == "green")   return {0,   128, 0,   255};
    if (c == "red")     return {255, 0,   0,   255};
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
    return {128, 128, 128, 255};
}