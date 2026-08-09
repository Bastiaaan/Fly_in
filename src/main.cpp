
#include "fly_in.hpp"
#include "system.hpp"

using namespace std;

int main()
{
    System system = System();
    string difficulty;
    int input;
    std::cout << "============ Choose a difficulty ============" << endl;
    std::cout << "easy (1), medium (2), hard (3), challenger(4)" << endl;
    std::cin >> input;
    switch (input)
    {
        case 1:
            difficulty = "easy";
            break;
        case 2:
            difficulty = "medium";
            break;
        case 3:
            difficulty = "hard";
            break;
        case 4:
            difficulty = "challenger";
            break;
        default:
            difficulty = "error";
            throw invalid_argument("invalid input.");
    }
    vector<tuple<int, string, string>> options = system.get_options(difficulty);
    std::cout << "=========== Choose a map " << '(' << difficulty << ')' << " ===========" << endl;
    for (auto &option : options)
        std::cout << get<0>(option) << ": " << get<1>(option) << endl;
    int chosenMap;
    std::cin >> chosenMap;
    if (chosenMap < get<0>(options[0]) ||
        chosenMap > get<0>(options[options.size() - 1]))
    {
        std::cout << "Invalid choice (" << chosenMap << "). try again" << endl;
        return 1;
    }
    auto result = system.Load(get<2>(options[chosenMap - 1]), difficulty);
    if (!result.success)
    {
        if (!result.line.has_value())
            std::cout << "So here's what happened: " << result.why.value() << endl;
        else
            std::cout << "Error while parsing: " << result.why.value() << " at line " << result.line.value() << endl;
    }
    else
    {
        system._map.setName(get<1>(options[chosenMap - 1]));
        system._map.difficulty = difficulty;
        auto droneRes = system.initDrones();
        auto algoResult = fly_in(system);
    }
	return 0;
};