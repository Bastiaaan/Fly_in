
#include "../reference/fly_in.hpp"

using namespace std;

int main(void)
{
    string difficulty;
    int input;
    cout << "============ Choose a difficulty ============" << endl;
    cout << "easy (1), medium (2), hard (3), challenger(4)" << endl;
    cin >> input;
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
    System system;
    vector<int> options = system.get_options(difficulty);
    for (int i = 0; i < options.size(); i++)
        cout << "Option #" << options[i] << endl;
	return 0;
};