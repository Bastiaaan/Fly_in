
#include "../raylib/include/raylib.h"
#include "../reference/fly_in.hpp"

static ExecuteState fly_in(System &sys)
{
    try
    {
        while(true)
        {
            std::cout << "Flying in the drones" << std::endl;
            break;
        }
        throw AlgoException("Oh oh, could not Fly-In the drones :(");
    }
    catch(AlgoException &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}