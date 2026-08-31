
#include "fly_in.hpp"
#include "system.hpp"

using namespace std;

int main()
{
	try
	{
		System system = System();
    	auto _json = system.jsonMap();
        if (!_json.success)
        {
            std::cerr << _json.why.value() << std::endl;
            std::cerr << "could not execute the simulator\nExiting (1)" << std::endl;
            return 1;
        }
        std::string file = std::any_cast<std::string>(_json.result);
        if (std::any_cast<std::string>(_json.result) == "fail.json")
        {
            std::cerr << "something went wrong during parsing\nExiting (2)" << std::endl;
            return 2;
        }
        ExecuteState loadRes = system.extractMap(file);
        if (!loadRes.success)
            throw std::logic_error(loadRes.why.value());
        auto init = system.initDrones();
        if (!init.success)
            throw AlgoException("something went wrong with drone-init");
        auto flied_in = fly_in(system);
        if (!flied_in.success)
            throw AlgoException("Fly-in failed");
		return 0;
	}
	catch(const AlgoException& ex)
	{
		std::cerr << ex.what() << '\n';
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
};
