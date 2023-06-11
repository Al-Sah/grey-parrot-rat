cmake_minimum_required(VERSION 3.22)

project(grey-parrot-rat
        DESCRIPTION "Additional project to build all cpp clients"
        )


option(BUILD_BOT_APP "Build 'bot' component" ON)
option(BUILD_OPERATOR_APP "Build app with GUI for the operator" ON)


include(cmake/manage-3d-parties.cmake)
include(cmake/utils.cmake)

# get 3d-parties
get_libdatachannel_project()
get_nlohmann_json_project()


check_platform_and_compiler()

# set(QT_DIR "C:/Qt/5.15.2/mingw81_64/")
# set(QT_DIR "/opt/Qt/5.15.2/gcc_64/")
# # pass as -DQT_DIR_PATH:STRING="/opt/Qt/5.15.2/gcc_64/"
SET(QT_DIR_PATH "" CACHE STRING "path to qt library")
find_qt_libraries()


add_subdirectory(modules-sdk)
add_subdirectory(modules)

add_subdirectory(rat-core-lib)



if(BUILD_BOT_APP)
    add_subdirectory(agent)
endif()

if(BUILD_OPERATOR_APP)
    add_subdirectory(operator-app)
endif()
