cmake_minimum_required(VERSION 3.17)
project(fly_in)

set(CMAKE_CXX_STANDARD 17)
file(GLOB SRCS "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/reference/*.cpp")
include_directories(reference)
include_directories(raylib/include)

if(APPLE)
    # Locate raylib library
    Find_Package ( raylib REQUIRED )
    # Include raylib Headers
    include_directories(${raylib_INCLUDE_DIR})
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/dylibs")
    get_filename_component(raylib_PATH ${raylib_LIBRARY} REALPATH)
    file(COPY ${raylib_PATH} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/dylibs")
    get_filename_component(raylib ${raylib_PATH} NAME)
    execute_process(COMMAND "install_name_tool -id @executable_path/../Frameworks/${raylib}.dylib ${raylib_LIBRARY}")
    set(raylib_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/dylibs/${raylib}")
endif(APPLE)

if(UNIX AND NOT APPLE)
    add_library(raylib SHARED IMPORTED)
    set_target_properties(raylib PROPERTIES
            IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/raylib/lib/libraylib.so")
endif()

add_executable(fly_in ${SRCS})
target_link_libraries(fly_in raylib)