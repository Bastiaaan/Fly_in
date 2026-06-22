
#include <iostream>
#include <string>

int main(void)
{
	int difficulty;
	int option;
	std::string path = "./../maps/";
	std::string choosen_diff = "";
	std::cout << "=== Let's fly in the drones ===" << std::endl;
	std::cout << "Choose a difficulty;\n\teasy (0), medium (1), hard (2) or challenger (3)" << std::endl;
	std::cin >> difficulty;
	switch (difficulty)
	{
		case 0:
			choosen_diff = "easy";
			path.append("easy/");
		break;
		case 1:
			choosen_diff = "medium";
			path.append("medium/");
		break;
		case 2:
			choosen_diff = "hard";
			path.append("hard/");
		break;
		case 3:
			choosen_diff = "challenger";
			path.append("challenger/");
		break;
		default:
			std::cerr << "Invalid choice" << std::endl;
			return 1;
		break;
	}
	std::cout << "Choose an option (" << choosen_diff << "):" << std::endl;
	std::cin >> option;

	return 0;
};