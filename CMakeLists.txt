cmake_minimum_required(VERSION 3.24)

project(grey-parrot-clients
        DESCRIPTION "Additional project to build all cpp clients"
        )


option(BUILD_BOT_APP "Build 'bot' component" ON)
option(BUILD_OPERATOR_APP "Build app with GUI for the operator" ON)


if(BUILD_BOT_APP)
    add_subdirectory(bot)
endif()


if(BUILD_OPERATOR_APP)
    add_subdirectory(operator-app)
endif()
