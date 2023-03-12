cmake_minimum_required(VERSION 3.5)

project(operator-app VERSION 0.0.1 LANGUAGES CXX)

# default properties
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)


configure_file(operator-app-version.h.in operator-app-version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})



add_executable(operator-app ${PROJECT_SOURCES})


find_package(Qt5 REQUIRED COMPONENTS Widgets)
target_link_libraries(operator-app PRIVATE Qt5::Widgets)
set_target_properties(operator-app PROPERTIES WIN32_EXECUTABLE TRUE)