cmake_minimum_required(VERSION 3.24)

project(core-msgs)

# TODO: if not found - use as external project
find_package(Protobuf REQUIRED)

message( DEBUG "PROTOBUF: include_dirs: ${Protobuf_INCLUDE_DIRS}")
message( DEBUG "PROTOBUF: dir with result: ${CMAKE_CURRENT_BINARY_DIR}")
message( DEBUG "PROTOBUF: libraries ${Protobuf_LIBRARIES}")

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS messages.proto)


add_library(core-msgs STATIC ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(core-msgs ${Protobuf_LIBRARIES})



