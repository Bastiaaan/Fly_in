cmake_minimum_required(VERSION 3.26)
project(fly_in)

set(CMAKE_CXX_STANDARD 17)

include_directories(reference)
add_executable(fly_in src/main.cpp)
