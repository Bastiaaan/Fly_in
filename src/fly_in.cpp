
#include <raylib.h>
#include "fly_in.hpp"

#include "visuals.hpp"

ExecuteState fly_in(System &sys)
{
    auto const start = sys._map.getHub("start");
    auto const end = sys._map.getHub("end");
    for (int i = 0; i < sys.nb_drones; i++)
    {
        auto *drone = new Drone(i+1);
        sys._map.available_drones.push_back(drone);
    }
    try
    {
        InitWindow(100, 100, "====== Fly-In ======");
        auto sizes = MeasureBank::Init(sys._map);
        SetWindowSize(sizes.screenWidth, sizes.screenHeight);
        SetWindowPosition(
        (GetMonitorWidth(sizes.monitorID)  - sizes.screenWidth)  / 2,
        (GetMonitorHeight(sizes.monitorID) - sizes.screenHeight) / 2
        );
        std::map<std::string, bool> checkList;
        checkList.insert({"prepare_background", false});
        checkList.insert({"horizontal_lines", false});
        checkList.insert({"vertical_lines", false});
        checkList.insert({"matching_crossing_pixels", false});
        checkList.insert({"locating_hubs", false});

        while(!WindowShouldClose())
        {
            BeginDrawing();
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
            //DrawText(TextFormat("Turns taken: %d", sys.turn), 20, 85, 30, DARKGRAY);

            unsigned long const total_x_radius = sizes.screenWidth;// - sizes.startActionRadius_x;
            unsigned long const total_y_radius = sizes.screenHeight;// endActionRadius_y - sizes.startActionRadius_y;
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
                         horr_line.endx,   horr_line.endy, LIGHTGRAY);

                if (!checkList["horizontal_lines"])
                {
                    for (int i = 0; i <= sizes.hubRangeX.size(); i++)
                    {
                        horr_line.pos_x = i;
                        sizes.saveLine(horr_line);
                    }
                }
                incr_y += step_y;
            }
            checkList["horizontal_lines"] = true;
            //if (!checkList["matching_crossing_pixels"])
            //{
            for (auto hub : sys._map.hubs)
            {
                auto hubLoc = Visualizer::locateHub(*hub, sizes.lines);
                float radius = 90;
                if (sys._map.difficulty == "easy") radius = 90;
                if (sys._map.difficulty == "medium") radius = 70;
                if (sys._map.difficulty == "hard") radius = 50;
                if (sys._map.difficulty == "challenger") radius = 30;
                if (hubLoc.x > 0 && hubLoc.y > 0)
                {
                    if (hub->zone.has_value())
                    {
                        if (hub->zone.value() == Zone::Blocked)
                        {
                            DrawPoly({static_cast<float>(hubLoc.x), static_cast<float>(hubLoc.y)},
                                      4, radius + 3, 45.0f, BLACK);
                            DrawPoly({static_cast<float>(hubLoc.x), static_cast<float>(hubLoc.y)},
                                      4, radius, 45.0f, Visualizer::resolveColor(*hub));
                        }
                        else if (hub->zone.value() == Zone::Restricted)
                        {
                            DrawPoly({static_cast<float>(hubLoc.x), static_cast<float>(hubLoc.y)},
                                      6, radius + 3, 0.0f, BLACK);
                            DrawPoly({static_cast<float>(hubLoc.x), static_cast<float>(hubLoc.y)},
                                      6, radius, 0.0f, Visualizer::resolveColor(*hub));
                        }
                        else if (hub->zone.value() == Zone::Priority)
                        {
                            DrawPoly({static_cast<float>(hubLoc.x), static_cast<float>(hubLoc.y)},
                                      4, radius + 3, 0.0f, BLACK);
                            DrawPoly({static_cast<float>(hubLoc.x), static_cast<float>(hubLoc.y)},
                                      4, radius, 0.0f, Visualizer::resolveColor(*hub));
                        }
                        else if (hub->zone.value() == Zone::Normal)
                        {
                            DrawCircle(hubLoc.x, hubLoc.y, radius + 3, BLACK);
                            DrawCircle(hubLoc.x, hubLoc.y, radius, Visualizer::resolveColor(*hub));
                        }
                    }
                    DrawText(TextFormat("%s", hub->name.c_str()), hubLoc.x - (radius / 2), hubLoc.y + radius, 30, BLACK);
                }
            }
            //}
            //checkList["matching_crossing_pixels"] = true;
            EndDrawing();
        }
        throw AlgoException("Oh oh, could not Fly-In the drones :(");
    }
    catch(AlgoException &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}
