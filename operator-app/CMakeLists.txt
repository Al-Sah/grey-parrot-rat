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


check_platform_and_compiler()


# set(QT_DIR "C:/Qt/5.15.2/mingw81_64/")
# set(QT_DIR "/opt/Qt/5.15.2/gcc_64/")
# # pass as -DQT_DIR_PATH:STRING="/opt/Qt/5.15.2/gcc_64/"
SET(QT_DIR_PATH "" CACHE STRING "path to qt library")
find_qt_libraries()

target_link_libraries(operator-app PRIVATE
        Qt${USING_QT}::Core
        Qt${USING_QT}::Widgets
        core-lib
        )

if (WIN32)
    set_target_properties(operator-app PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
