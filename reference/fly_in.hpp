
#ifndef FLY_IN_HPP
# define FLY_IN_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "hub.hpp"
#include "data.hpp"
#include "drone.hpp"
#include "system.hpp"
#include "map.hpp"

vector<string> split(const string &s, char delimiter, unsigned int size = 0);
string join(const vector<string>& parts, char delimiter, unsigned int size = 0);

#endif