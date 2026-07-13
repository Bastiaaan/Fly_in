
#ifndef FLY_IN_HPP
# define FLY_IN_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "abstractions.hpp"
#include "hub.hpp"
#include "connection.hpp"
#include "factory.hpp"
#include "drone.hpp"
#include "system.hpp"
#include "map.hpp"

vector<string> split(const string &s, char delimiter, unsigned int size = 0);
string join(const vector<string>& parts, char delimiter, unsigned int size = 0);

#endif