
#include <raylib.h>
#include "fly_in.hpp"

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
        constexpr unsigned int screenWidth = 2843;
        constexpr unsigned int screenHeight = 1650;
        InitWindow(screenWidth, screenHeight, "====== Fly-In ======");
        auto coordRange =  [sys](char const c) -> int
        {
            if (c == 'x' || c == 'y')
            {
                int max = 0, min = 0;
                for (int x = 0; x < sys._map.hubs.size(); x++)
                {
                    const int current = (c == 'x')
                        ? sys._map.hubs[x]->position_x
                        : sys._map.hubs[x]->position_y;
                    if (current > max)
                        max = current;
                    if (current < min)
                        min = current;
                }
                return (max - min) + 1;
            }
            return -1;
        };
        while(!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawLine(0, 0, screenWidth, screenHeight, RED);
            DrawLine(0, screenHeight, screenWidth, 0, BLUE);
            DrawText("Map name: ", 20, 30, 55, DARKGREEN);
            DrawText(TextFormat("Turns taken: %d", sys.turn), 20, 85, 30, DARKGRAY);
            sys.turn++;
            EndDrawing();
            if (sys.turn == 2501)
                CloseWindow();
        }
        //CloseWindow();
        throw AlgoException("Oh oh, could not Fly-In the drones :(");
    }
    catch(AlgoException &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}