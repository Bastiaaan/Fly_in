
#include <raylib.h>
#include <map>

#include "algorithm.hpp"
#include "fly_in.hpp"
#include "visuals.hpp"

ExecuteState fly_in(System &sys)
{
    try
    {
    	auto const it = std::find_if(
    		sys._map.hubs.begin(),
    		sys._map.hubs.end(),
    		[](Hub const *hub) { return hub->isEnd(); }
		);
		Hub const *end = (it != sys._map.hubs.end()) ? *it : nullptr;

        InitWindow(100, 100, "========== Fly-In ==========");
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
		for (unsigned int n = 0; n < 2; n++)
			std::cout << std::endl;
		std::cout << "========== begin Simulation ==========" << std::endl << std::endl;
        while(!WindowShouldClose() || end ->drones.size() != sys.nb_drones)
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
				int rotate = Algorithm::rotateDrones(sys, sizes.hubRadius.x);
				if (rotate == 0)
				{
					DrawText(
						"Simulation got stuck, quiting soon :(",
						sizes.screenWidth / 2 - 500,
						sizes.startActionRadius_y + 40, 50, RED);
                    WaitTime(1.5);
            		EndDrawing();
					std::cout << std::endl << "========== end Simulation ==========";
					for (unsigned int n = 0; n < 3; n++)
						std::cout << std::endl;
					CloseWindow();
					break;
				}
                sys.turn += rotate;
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
					std::cout << std::endl << "========== end Simulation ==========";
					for (unsigned int n = 0; n < 3; n++)
						std::cout << std::endl;
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

/*
Please ask the reviewee to add a simple modification to their drone simulation program.
Ask them to add a "--capacity-info" flag that displays capacity information during simulation,
such as "Zone X: Y/Z drones, Connection A-B: Y/Z capacity used" for each turn.

For example, "./main.py --capacity-info map.txt" should output the normal simulation plus
capacity usage information. The reviewee should be able to locate the relevant
parsing and output code, make the necessary modifications, and demonstrate that it works
with a test case. The entire task, including the demonstration, should take no more
than 10 minutes. Was this procedure followed and did everything work correctly?
*/
