//
// Created by brogaar on 29-6-26.
//

#include "../../reference/fly_in.hpp"

Hub::Hub(Map map, int x, int y, std::string name)
{
    this->map = map;
    this->position_x = x;
    this->position_y = y;
    this->name = name;
};

Hub::~Hub()
{

}