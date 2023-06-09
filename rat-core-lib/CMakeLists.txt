cmake_minimum_required(VERSION 3.22)

project(core-lib)

# TODO: if not found - use as external project
find_package(Protobuf REQUIRED)

message( STATUS "PROTOBUF: include_dirs: ${Protobuf_INCLUDE_DIRS}")
message( STATUS "PROTOBUF: dir with result: ${CMAKE_CURRENT_BINARY_DIR}")
message( STATUS "PROTOBUF: libraries: ${Protobuf_LIBRARIES}")


get_target_property(MODULES_SDK_INCLUDES modules-sdk INCLUDE_DIRECTORIES)

include_directories(
        ${Protobuf_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR} # generated proto files
        ${LIBDATACHANNEL_INCLUDES}
        ${MODULES_SDK_INCLUDES})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/core-data.proto proto/control.proto)



set(CORE_SOURCES
        networking/ConnectionsManager.cpp
        networking/C2ServerChannel.cpp

        tasks-managing/TasksMap.cpp
        tasks-managing/TasksMapper.cpp
        tasks-managing/TasksManagerBase.cpp

        CoreUtils.cpp)

set(CORE_HEADERS
        IControlPacketSender.h
        IControlPacketHandler.h
        ITaskHandler.h

        networking/ChannelState.h
        networking/C2ServerChannel.h
        networking/ConnectionsManager.h
        networking/PeerData.h

        tasks-managing/TasksManagerBase.h
        tasks-managing/TasksMap.h
        tasks-managing/TasksMapper.h

        CoreUtils.h)


add_library(core-lib STATIC
        ${CORE_SOURCES}
        ${CORE_HEADERS}
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        )

target_link_libraries(core-lib
        libdatachannel
        modules-sdk
        ${Protobuf_LIBRARIES}
        )

add_dependencies(core-lib sd-party-libdatachannel)