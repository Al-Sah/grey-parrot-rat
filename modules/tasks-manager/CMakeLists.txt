
cmake_minimum_required(VERSION 3.22)

project(tasks-manager-module VERSION 0.0.1 LANGUAGES CXX)


# default properties
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(TASKS_MANAGER_MODULE_AGENT_SOURCES
        TasksManagerAgent.cpp
        TasksManagerAgent.h
        TaskDescription.h)

set(TASKS_MANAGER_MODULE_OPERATOR_SOURCES
        ProcessManagerOperator.cpp
        ProcessManagerOperator.h

        TaskDescription.h

        TaskForm.cpp
        TaskForm.h
        TaskForm.ui

        RunningTasksWidget.cpp
        RunningTasksWidget.h
        RunningTasksWidget.ui)

get_target_property(MODULES_SDK_INCLUDES modules-sdk INCLUDE_DIRECTORIES)
include_directories(${MODULES_SDK_INCLUDES})


add_library(tasks-manager-module-agent STATIC ${TASKS_MANAGER_MODULE_AGENT_SOURCES})
add_library(tasks-manager-module-operator STATIC ${TASKS_MANAGER_MODULE_OPERATOR_SOURCES})


target_link_libraries(tasks-manager-module-operator PRIVATE
        Qt${USING_QT}::Core
        Qt${USING_QT}::Widgets
        procps
        )


