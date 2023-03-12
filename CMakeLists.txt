cmake_minimum_required(VERSION 3.24)

project(grey-parrot-clients
        DESCRIPTION "Additional project to build all cpp clients"
        )


# TODO add options what to build
add_subdirectory(bot)
add_subdirectory(operator-app)