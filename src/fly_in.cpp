
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
        checkList.insert({"saving_lines", false});
        checkList.insert({"vertical_lines", false});
        checkList.insert({"horizontal_lines", false});
        checkList.insert({"connecting_hubs", false});
        Vector2 drone = {0, static_cast<float>(sizes.screenHeight / 2)};
        while(!WindowShouldClose())
        {
            BeginDrawing();
            Renderer::renderBackground(sys, sizes);
            Renderer::renderBackgroundLines(sys, sizes, checkList);
            Renderer::renderConnections(sys, sizes);
            Renderer::renderHubs(sys, sizes);
            Renderer::renderDrones(sys, sizes);
            DrawPoly(drone, 4, 30, 45.0f, GRAY);
            drone.x++;
            WaitTime(0.00105);
            EndDrawing();
        }
        throw AlgoException("Oh oh, could not Fly-In the drones :(");
    }
    catch(AlgoException &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}
