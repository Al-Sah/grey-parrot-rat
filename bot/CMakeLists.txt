cmake_minimum_required(VERSION 3.24)

project(gray-parrot-bot VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


configure_file(app-info/bot-version.h.in bot-version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# sd-party libs
include_directories(
        ${LIBDATACHANNEL_INCLUDES}
        ${NLOHMANN_JSON_INCLUDES}
        #${WEBSOCKETPP_INCLUDES}
        )

add_subdirectory(../modules-sdk sdk)
include_directories(../modules-sdk)

get_target_property(core_msgs_includes core-msgs INCLUDE_DIRECTORIES)
include_directories(${core_msgs_includes})


set(BOT_COMMON_SOURCES
        main.cpp
        Bot.cpp
        app-info/ApplicationDetails.cpp

        core/networking/ConnectionsManager.cpp
        core/networking/ControlMessageHeader.cpp
        core/networking/C2ServerChannel.cpp
        core/tasks-managing/TasksManager.cpp
        core/tasks-managing/TasksMap.cpp
        core/TasksCreator.cpp
        core/modules-managing/ModulesManager.cpp
        )


set(BOT_COMMON_HEADERS
        Bot.h
        app-info/platform.h
        platform-info/DeviceDetails.h
        app-info/ApplicationDetails.h

        core/TasksCreator.h

        core/networking/ConnectionsManager.h
        core/networking/ControlMessageHeader.h
        core/networking/ChannelState.h
        core/networking/C2ServerChannel.h
        core/networking/IMessagesSender.h

        core/tasks-managing/TasksManager.h
        core/tasks-managing/IResultHandler.h
        core/tasks-managing/TasksMap.h
        core/tasks-managing/TaskInfo.h

        core/modules-managing/ModulesManager.h
        core/modules-managing/ITaskDelegator.h
        )


if (WIN32)
    set(BOT_PLATFORM_DEPEND_SOURCES platform-info/collectors/WinInfoCollector.cpp)
    set(BOT_PLATFORM_DEPEND_HEADERS platform-info/collectors/WinInfoCollector.h)
else()
    set(BOT_PLATFORM_DEPEND_SOURCES platform-info/collectors/LinuxInfoCollector.cpp)
    set(BOT_PLATFORM_DEPEND_HEADERS platform-info/collectors/LinuxInfoCollector.h)
endif (WIN32)


add_executable(gray-parrot-bot
        ${BOT_COMMON_SOURCES}
        ${BOT_PLATFORM_DEPEND_SOURCES}

        ${BOT_COMMON_HEADERS}
        ${BOT_PLATFORM_DEPEND_HEADERS}
        )

target_link_libraries(gray-parrot-bot gray-parrot-modules-sdk libdatachannel core-msgs)

add_dependencies(gray-parrot-bot sd-party-libdatachannel)
add_dependencies(gray-parrot-bot sd-party-nlohmann-json)
#add_dependencies(gray-parrot-bot sd-party-websocketpp)
