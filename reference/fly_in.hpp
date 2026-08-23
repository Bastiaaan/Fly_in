#pragma once

#include <cmath>
#include <filesystem>
#include <iostream>
#include <random>
#include <regex>
#include <optional>
#include <string>
#include <unordered_set>
#include <thread>
#include <variant>
#include <vector>

#include "algorithm.hpp"
#include "customExceptions.hpp"
#include "connection.hpp"
#include "drone.hpp"
#include "hub.hpp"
#include "factory.hpp"
#include "map.hpp"
#include "validator.hpp"
#include "system.hpp"

static std::mt19937 rng(std::random_device{}());
struct ExecuteState;
class System;

std::vector<std::string> split(const std::string &s, char delimiter, unsigned int size = 0);
std::string join(const std::vector<std::string>& parts, char delimiter, unsigned int size = 0);
bool isNumeric(std::string const &s);
ExecuteState fly_in(System &sys);
