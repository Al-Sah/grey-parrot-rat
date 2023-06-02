cmake_minimum_required(VERSION 3.22)

project(grey-parrot-clients
        DESCRIPTION "Additional project to build all cpp clients"
        )


option(BUILD_BOT_APP "Build 'bot' component" ON)
option(BUILD_OPERATOR_APP "Build app with GUI for the operator" ON)


include(cmake/manage-3d-parties.cmake)

# get 3d-parties
get_libdatachannel_project()
get_nlohmann_json_project()


add_subdirectory(modules-sdk)
add_subdirectory(rat-core-lib)



if(BUILD_BOT_APP)
    add_subdirectory(bot)
endif()

if(BUILD_OPERATOR_APP)
    add_subdirectory(operator-app)
endif()
