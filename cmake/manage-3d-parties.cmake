

include(ExternalProject)


macro(get_libdatachannel_project)

    message(STATUS "Configuring external libdatachannel project ... ")

    set(LIBDATACHANNEL_PROJECT  "${CMAKE_CURRENT_BINARY_DIR}/libdatachannel")
    set(LIBDATACHANNEL_INCLUDES "${LIBDATACHANNEL_PROJECT}/res/include")
    set(LIBDATACHANNEL_LIB_NAME "${CMAKE_SHARED_LIBRARY_PREFIX}datachannel${CMAKE_SHARED_LIBRARY_SUFFIX}")


    if(WIN32)
        set(LIBDATACHANNEL_BINARIES "${LIBDATACHANNEL_PROJECT}/res/bin")
        set(BUILD_BYPRODUCTS_VALUE "${LIBDATACHANNEL_BINARIES}/${LIBDATACHANNEL_LIB_NAME}")
        if(MINGW)
            list(APPEND BUILD_BYPRODUCTS_VALUE ";${LIBDATACHANNEL_PROJECT}/res/lib/libdatachannel.dll.a")
        endif()
    else()
        set(LIBDATACHANNEL_BINARIES "${LIBDATACHANNEL_PROJECT}/res/lib")
        set(BUILD_BYPRODUCTS_VALUE "${LIBDATACHANNEL_BINARIES}/${LIBDATACHANNEL_LIB_NAME}")
    endif()

    set(LIBDATACHANNEL_CMAKE_ARGS
            # environment
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
            -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> # install in cmake-build folder

            # project (libdatachannel) specific options
            -DNO_EXAMPLES:BOOL=ON
            -DNO_TESTS:BOOL=ON
            )

    if(WIN32)
        # OPENSSL_PATH can be something like "C:/Program\ Files/FireDaemon\ OpenSSL\ 3"
        if(NOT OPENSSL_PATH STREQUAL "")
            message(STATUS "Passing OPENSSL_ROOT_DIR variable to libdatachannel project: \"${OPENSSL_PATH}\"")
            list(APPEND LIBDATACHANNEL_CMAKE_ARGS -DOPENSSL_ROOT_DIR:PATH=${OPENSSL_PATH})
        endif()

        if(MINGW)
            # for libsrtp2 because it cant be normally compiled with mingw; Bug ...
            list(APPEND LIBDATACHANNEL_CMAKE_ARGS -DBUILD_WITH_WARNINGS:BOOL=OFF)
        endif()
        if(CROSSCOMPILING_ON_LINUX_WITH_MINGW)
            # TODO Fix it; cross-compilation is not available yet
            list(APPEND LIBDATACHANNEL_CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE:PATH=${PROJECT_SOURCE_DIR}/cmake/linux-x86_64-w64-mingw32-toolcahin.cmake)
        endif()
    endif()


    ExternalProject_Add(sd-party-libdatachannel

            # directory options
            PREFIX "${LIBDATACHANNEL_PROJECT}" # root directory for the external (libdatachannel) project
            STAMP_DIR       "${LIBDATACHANNEL_PROJECT}/stamp"
            SOURCE_DIR      "${LIBDATACHANNEL_PROJECT}/src"
            BINARY_DIR      "${LIBDATACHANNEL_PROJECT}/bin"
            INSTALL_DIR     "${LIBDATACHANNEL_PROJECT}/res"
            LOG_DIR         "${LIBDATACHANNEL_PROJECT}/log"

            # Download step (using git)
            GIT_REPOSITORY  "https://github.com/paullouisageneau/libdatachannel"
            GIT_REMOTE_NAME "origin"
            GIT_TAG         "refs/tags/v0.18.4"
            # GIT_SUBMODULES ??
            GIT_SUBMODULES_RECURSE ON
            GIT_PROGRESS    ON
            GIT_SHALLOW     ON  # Get just specified commit (GIT_TAG)

            # Configure step
            #CMAKE_GENERATOR: the same generator as the main build
            CMAKE_ARGS          "${LIBDATACHANNEL_CMAKE_ARGS}"

            UPDATE_COMMAND ""

            #Build step (BUILD_BYPRODUCTS must be specified when 'ninja' is used)
            # TODO some strange magic happened here; I want to build target datachannel-static
            #  default build command is: "cmake;--build;." with ';' ???
            #  it fails with "${CMAKE_COMMAND} --build . --target datachannel-static"
            # BUILD_COMMAND       "${CMAKE_COMMAND};--build;.;--target datachannel-static" - passed
            # TODO: Use static version of this library
            BUILD_BYPRODUCTS    "${BUILD_BYPRODUCTS_VALUE}"
            # https://stackoverflow.com/questions/54866067/cmake-and-ninja-missing-and-no-known-rule-to-make-it

            # Test step
            TEST_COMMAND "" # Run without tests

            # Log
            LOG_DOWNLOAD    ON
            LOG_CONFIGURE   ON
            LOG_BUILD       ON
            LOG_INSTALL     ON
            )

    add_library(libdatachannel SHARED IMPORTED)

    #set_target_properties(libdatachannel PROPERTIES IMPORTED_LOCATION ${LIBDATACHANNEL_BINARIES}/libdatachannel.so)
    # This project does not provide install option for the static library, so use dynamic (fix it in future)
    if(WIN32)
        set_target_properties(libdatachannel PROPERTIES
                IMPORTED_LOCATION "${LIBDATACHANNEL_BINARIES}/${LIBDATACHANNEL_LIB_NAME}"
                IMPORTED_IMPLIB "${LIBDATACHANNEL_PROJECT}/res/lib/libdatachannel.dll.a"
                )
    else()
        set_target_properties(libdatachannel PROPERTIES
                IMPORTED_LOCATION "${LIBDATACHANNEL_BINARIES}/${LIBDATACHANNEL_LIB_NAME}"
                )
    endif()

endmacro()



macro(get_nlohmann_json_project)

    message(STATUS "Configuring external nlohmann_json project ... ")

    set(NLOHMANN_JSON_PROJECT   "${CMAKE_CURRENT_BINARY_DIR}/nlohmann-json")
    set(NLOHMANN_JSON_INCLUDES  "${NLOHMANN_JSON_PROJECT}/res/include")

    set(NLOHMANN_JSON_CMAKE_ARGS
            # environment
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
            -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> # install in cmake-build folder

            # project (nlohmann-json) specific options
            # ???
            )

    # Configure nlohmann json as an external project
    ExternalProject_Add(sd-party-nlohmann-json

            # directory options
            PREFIX "${NLOHMANN_JSON_PROJECT}" # root directory for the external (nlohmann-json) project
            STAMP_DIR       "${NLOHMANN_JSON_PROJECT}/stamp"
            SOURCE_DIR      "${NLOHMANN_JSON_PROJECT}/src"
            BINARY_DIR      "${NLOHMANN_JSON_PROJECT}/bin"
            INSTALL_DIR     "${NLOHMANN_JSON_PROJECT}/res"
            LOG_DIR         "${NLOHMANN_JSON_PROJECT}/log"

            GIT_REPOSITORY  "https://github.com/nlohmann/json.git"
            GIT_REMOTE_NAME "origin"
            GIT_TAG         "refs/tags/v3.10.4"
            GIT_PROGRESS    ON
            GIT_SHALLOW     ON  # Get just specified commit (GIT_TAG)

            CMAKE_ARGS      "${NLOHMANN_JSON_CMAKE_ARGS}"

            UPDATE_COMMAND ""

            # Test step
            TEST_COMMAND "" # Run without tests

            # This library is header only

            # Log
            LOG_DOWNLOAD    ON
            LOG_CONFIGURE   ON
            LOG_BUILD       ON
            LOG_INSTALL     ON
            )

endmacro()

# TODO: replace libdatachannel websocket with websocketpp (add asio + fix for c++20)
#[[
macro(get_websocketpp_project)

    message(STATUS "Configuring external websocketpp project ... ")

    set(WEBSOCKETPP_PROJECT   "${CMAKE_CURRENT_BINARY_DIR}/websocketpp")
    set(WEBSOCKETPP_INCLUDES  "${WEBSOCKETPP_PROJECT}/res/include")

    set(WEBSOCKETPP_CMAKE_ARGS
            # environment
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
            -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> # install in cmake-build folder

            # project (websocketpp) specific options
            # ???
            )

    # Configure websocketpp as an external project
    ExternalProject_Add(sd-party-websocketpp

            # directory options
            PREFIX "${WEBSOCKETPP_PROJECT}" # root directory for the external (nlohmann-json) project
            STAMP_DIR       "${WEBSOCKETPP_PROJECT}/stamp"
            SOURCE_DIR      "${WEBSOCKETPP_PROJECT}/src"
            BINARY_DIR      "${WEBSOCKETPP_PROJECT}/bin"
            INSTALL_DIR     "${WEBSOCKETPP_PROJECT}/res"
            LOG_DIR         "${WEBSOCKETPP_PROJECT}/log"

            GIT_REPOSITORY  "https://github.com/zaphoyd/websocketpp.git"
            GIT_REMOTE_NAME "origin"
            GIT_TAG         "refs/tags/0.8.2"
            GIT_PROGRESS    ON
            GIT_SHALLOW     ON  # Get just specified commit (GIT_TAG)

            CMAKE_ARGS      "${WEBSOCKETPP_CMAKE_ARGS}"

            UPDATE_COMMAND ""

            # Test step
            TEST_COMMAND "" # Run without tests

            # This library is header only

            # Log
            LOG_DOWNLOAD    ON
            LOG_CONFIGURE   ON
            LOG_BUILD       ON
            LOG_INSTALL     ON
            )

endmacro()
]]
