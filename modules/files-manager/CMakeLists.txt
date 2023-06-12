
cmake_minimum_required(VERSION 3.22)

project(files-manager-module VERSION 0.0.1 LANGUAGES CXX)


# default properties
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(FILES_MANAGER_MODULE_AGENT_SOURCES
        FilesManagerAgent.cpp
        FilesManagerAgent.h)

set(FILES_MANAGER_MODULE_OPERATOR_SOURCES
        FilesManagerOperatorApp.cpp
        FilesManagerOperatorApp.h

        FilesManagerWidget.cpp
        FilesManagerWidget.h
        FilesManagerWidget.ui
        )

get_target_property(MODULES_SDK_INCLUDES modules-sdk INCLUDE_DIRECTORIES)
include_directories(${MODULES_SDK_INCLUDES})


add_library(files-manager-module-agent STATIC ${FILES_MANAGER_MODULE_AGENT_SOURCES})
add_library(files-manager-module-operator STATIC ${FILES_MANAGER_MODULE_OPERATOR_SOURCES})


target_link_libraries(files-manager-module-operator PRIVATE
        Qt${USING_QT}::Core
        Qt${USING_QT}::Widgets
        )


