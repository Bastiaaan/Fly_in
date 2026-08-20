
#include <raylib.h>
#include <map>

#include "algorithm.hpp"
#include "fly_in.hpp"
#include "visuals.hpp"

ExecuteState fly_in(System &sys)
{
    auto const end = sys._map.getHub("goal") == nullptr ?
					 sys._map.getHub("impossible_goal") :
					 sys._map.getHub("goal");
					
    for (unsigned long i = 0; i < sys.nb_drones; i++)
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
        while(!WindowShouldClose())
        {
            BeginDrawing();
            Renderer::renderBackground(sys, sizes);
            Renderer::renderBackgroundLines(sizes, checkList);
            Renderer::renderConnections(sys, sizes);
            Renderer::renderHubs(sys, sizes);
            Renderer::renderDrones(sys, sizes);
            if (Algorithm::noDroneFlies(sys) &&
				end->drones.size() != sys.nb_drones)
                sys.turn += Algorithm::rotateDrones(sys, sizes.hubRadius.x);
            else
                Algorithm::movingDrones(sys);
            EndDrawing();
            if (end->drones.size() == sys.nb_drones)
            {
                //CloseWindow();
            }
        }
        throw AlgoException("Oh oh, could not Fly-In the drones :(");
    }
    catch(AlgoException &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}
