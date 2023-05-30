cmake_minimum_required(VERSION 3.22)

project(gray-parrot-modules-sdk VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(SDK_SOURCES TaskExecutor.cpp)


set(SDK_HEADERS
        Task.h
        TaskResult.h
        TaskExecutor.h
        ModuleInfo.h
        )

add_library(gray-parrot-modules-sdk STATIC ${SDK_HEADERS} ${SDK_SOURCES})
