cmake_minimum_required(VERSION 3.26)
project(fly_in)

set(CMAKE_CXX_STANDARD 17)

include_directories(reference)
add_executable(fly_in src/main.cpp
        reference/fly_in.hpp
        src/reference/map.cpp
        src/reference/drone.cpp
        src/reference/hub.cpp
        src/reference/system.cpp
        src/utils.cpp
        reference/connection.hpp
        src/reference/connection.cpp
        src/reference/factory.cpp
        reference/abstractions.hpp
)
