cmake_minimum_required(VERSION 3.20)

project(operator-app VERSION 0.0.1 LANGUAGES CXX)

include(../cmake/utils.cmake)

# default properties
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_target_property(core_lib_includes core-lib INCLUDE_DIRECTORIES)
message(STATUS "core_lib_includes ${core_lib_includes}")
include_directories(${core_lib_includes})


set(PROJECT_SOURCES
        main.cpp

        core/OperatorModulesManager.h
        core/OperatorModulesManager.cpp
        core/OperatorTasksManager.cpp
        core/OperatorTasksManager.h

        gui/MainWindow.cpp
        gui/MainWindow.h
        gui/MainWindow.ui

        gui/AgentDescriptionListItem.cpp
        gui/AgentDescriptionListItem.h
        gui/AgentDescriptionListItem.ui

        gui/ItemsListDialog.cpp
        gui/ItemsListDialog.h
        gui/ItemsListDialog.ui

        OperatorApp.cpp
        OperatorApp.h

        CoreCtrlBridge.cpp
        CoreCtrlBridge.h
        ConnectionChangeInfo.h)


configure_file(operator-app-version.h.in operator-app-version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(operator-app ${PROJECT_SOURCES})

target_link_libraries(operator-app PRIVATE
        Qt${USING_QT}::Core
        Qt${USING_QT}::Widgets
        core-lib
        echo-module-operator
        )

if (WIN32)
    set_target_properties(operator-app PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
