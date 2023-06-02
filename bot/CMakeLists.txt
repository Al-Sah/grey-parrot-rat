cmake_minimum_required(VERSION 3.22)

project(gray-parrot-bot VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


configure_file(app-info/bot-version.h.in bot-version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

get_target_property(core_lib_includes core-lib INCLUDE_DIRECTORIES)
message(STATUS "core_lib_includes ${core_lib_includes}")
include_directories(${core_lib_includes})


set(BOT_COMMON_SOURCES
        main.cpp
        Bot.cpp
        app-info/ApplicationDetails.cpp

        core/AgentModulesManager.cpp
        core/AgentTasksManager.cpp
        )


set(BOT_COMMON_HEADERS
        Bot.h
        app-info/platform.h
        platform-info/DeviceDetails.h
        app-info/ApplicationDetails.h

        core/AgentModulesManager.h
        core/AgentTasksManager.h
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

target_link_libraries(gray-parrot-bot core-lib)