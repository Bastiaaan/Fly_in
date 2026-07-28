#pragma once

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

#include "customExceptions.hpp"
#include "connection.hpp"
#include "drone.hpp"
#include "hub.hpp"
#include "factory.hpp"
#include "system.hpp"
#include "map.hpp"
#include "validator.hpp"
#include "system.hpp"


vector<string> split(const string &s, char delimiter, unsigned int size = 0);
string join(const vector<string>& parts, char delimiter, unsigned int size = 0);

