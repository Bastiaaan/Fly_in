//
// Created by brogaar on 30-6-26.
//

#include "../../reference/fly_in.hpp"

void Data::read_data(std::string &path)
{
    std::cout << "The void does nothing at this moment" << std::endl;
}

std::string Data::get_path(int difficulty, int option)
{
    return "To be implemented";
}

bool Data::map_exists(std::string &difficulty, int level)
{
    std::string path;
    if (level > 0 && level < 10)
        path = "../../maps/0" + std::to_string(level);
    else if (level >= 10)
        path = "../../maps/" + std::to_string(level);
    else
        return false;
    try
    {
        std::ifstream possibleFound(path + "*.txt");
        possibleFound.close();
        return true;
    }
    catch (std::exception ex)
    {
		std::cout << "Map load failed: " << ex.what() << std::endl;
        return false;
    }
}
