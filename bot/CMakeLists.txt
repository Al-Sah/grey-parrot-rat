cmake_minimum_required(VERSION 3.24)

project(gray-parrot-bot VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


configure_file(bot-version.h.in bot-version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_executable(gray-parrot-bot main.cpp Bot.cpp Bot.h)
