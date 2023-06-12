
cmake_minimum_required(VERSION 3.22)

project(modules DESCRIPTION "Additional target to build all modules")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(echo)
add_subdirectory(streamer)
add_subdirectory(files-manager)
add_subdirectory(tasks-manager)