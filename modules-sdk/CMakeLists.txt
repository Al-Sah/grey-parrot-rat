cmake_minimum_required(VERSION 3.22)

project(modules-sdk VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(
        ${Protobuf_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBDATACHANNEL_INCLUDES}
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS general-data.proto)


set(SDK_SOURCES
        TaskExecutor.cpp
        TaskGenerator.cpp
        )

set(SDK_HEADERS
        Task.h
        TaskExecutor.h
        TaskGenerator.h
        ModuleInfo.h
        )

add_library(modules-sdk STATIC ${SDK_HEADERS} ${SDK_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
