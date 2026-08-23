
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
		for (unsigned int n = 0; n < 10; n++)
			std::cout << std::endl;
        while(!WindowShouldClose() || end->drones.size() != sys.nb_drones)
        {
            BeginDrawing();
            Renderer::renderBackground(sys, sizes);
            Renderer::renderBackgroundLines(sizes, checkList);
            Renderer::renderConnections(sys, sizes);
            Renderer::renderHubs(sys, sizes);
            Renderer::renderDrones(sys, sizes);
            if (Algorithm::noDroneFlies(sys) &&
				end->drones.size() != sys.nb_drones)
			{
                sys.turn += Algorithm::rotateDrones(sys, sizes.hubRadius.x);
				sys.verboseLog(sys.logs.size());
			}
            else
                Algorithm::movingDrones(sys);
            if (end->drones.size() == sys.nb_drones)
            {
				if (std::any_of(end->drones.begin(), end->drones.end(),
					[](Drone const *drone) -> bool {
						return drone->flying();
					}))
					Algorithm::movingDrones(sys);
				else
				{
					DrawText(
						"Simulation complete! Until next time :)",
						sizes.screenWidth / 2 - 500,
						sizes.startActionRadius_y + 40, 50, LIME);
            		EndDrawing();
                    WaitTime(1.5);
					CloseWindow();
				}
            }
			if (!WindowShouldClose())
            	EndDrawing();
        }
		return ExecuteState::Ok("all drones have reached the end :)");
    }
    catch(AlgoException &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}
