
cmake_minimum_required(VERSION 3.22)

project(streamer-module VERSION 0.0.1 LANGUAGES CXX)


# default properties
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(STREAMER_MODULE_AGENT_SOURCES
        StreamsProducer.cpp
        StreamsProducer.h)

set(STREAMER_MODULE_OPERATOR_SOURCES
        StreamsConsumer.cpp
        StreamsConsumer.h

        StreamerWidget.cpp
        StreamerWidget.h
        StreamerWidget.ui
        )

get_target_property(MODULES_SDK_INCLUDES modules-sdk INCLUDE_DIRECTORIES)
include_directories(${MODULES_SDK_INCLUDES})


add_library(streamer-module-agent STATIC ${STREAMER_MODULE_AGENT_SOURCES})
add_library(streamer-module-operator STATIC ${STREAMER_MODULE_OPERATOR_SOURCES})


target_link_libraries(streamer-module-operator PRIVATE
        Qt${USING_QT}::Core
        Qt${USING_QT}::Widgets
        )


