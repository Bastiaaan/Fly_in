
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
        auto const &sizes = MeasureBank::Init(sys._map);
        SetWindowSize(sizes.screenWidth, sizes.screenHeight);
        SetWindowPosition(
        (GetMonitorWidth(sizes.monitorID)  - sizes.screenWidth)  / 2,
        (GetMonitorHeight(sizes.monitorID) - sizes.screenHeight) / 2
        );
        while(!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawLine(sizes.actionRadius_x, 0, (sizes.screenWidth / 15), sizes.screenWidth, DARKGRAY);
            DrawRectangle(0, 0, (sizes.screenWidth / 15), sizes.screenHeight, LIGHTGRAY);
            DrawRectangle(0, sizes.actionRadius_y, sizes.screenWidth, sizes.screenHeight / 15, LIGHTGRAY);
            DrawRectangle(sizes.actionRadius_x, 0, (sizes.screenWidth / 15), sizes.screenHeight, LIGHTGRAY);
            DrawRectangle(0, 0, sizes.screenWidth, sizes.actionRadius_y, LIGHTGRAY);
            DrawLine(0, sizes.actionRadius_y, sizes.screenWidth, sizes.actionRadius_y, DARKGRAY);
            DrawLine(0, sizes.actionRadius_y, sizes.screenWidth, sizes.actionRadius_y, DARKGRAY);
            DrawLine(sizes.actionRadius_x, 0, sizes.actionRadius_x, sizes.screenHeight, DARKGRAY);
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
