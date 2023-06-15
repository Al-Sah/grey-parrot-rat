
cmake_minimum_required(VERSION 3.22)

project(echo-module VERSION 0.0.1 LANGUAGES CXX)


# default properties
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(ECHO_MODULE_AGENT_SOURCES
        EchoHandler.cpp
        EchoHandler.h)

set(ECHO_MODULE_OPERATOR_SOURCES
        EchoMaker.cpp
        EchoMaker.h

        EchoWidget.ui
        EchoWidget.h
        EchoWidget.cpp

        EchoModuleBridge.cpp
        EchoModuleBridge.h
        )

get_target_property(MODULES_SDK_INCLUDES modules-sdk INCLUDE_DIRECTORIES)
include_directories(${MODULES_SDK_INCLUDES})


add_library(echo-module-agent STATIC ${ECHO_MODULE_AGENT_SOURCES})
add_library(echo-module-operator STATIC ${ECHO_MODULE_OPERATOR_SOURCES})


target_link_libraries(echo-module-operator PRIVATE
        Qt${USING_QT}::Core
        Qt${USING_QT}::Widgets
        )


add_dependencies(echo-module-agent modules-sdk)
add_dependencies(echo-module-operator modules-sdk)
