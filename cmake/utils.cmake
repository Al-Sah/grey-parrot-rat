
# !!!!!
# This macro relies on QT_DIR_PATH "CACHE STRING" variable that can be passed as parameter
macro(find_qt_libraries)

    if(QT_DIR_PATH STREQUAL "")
        message(STATUS "QT_DIR_PATH option was not passed; Checking environment variables ...")
        set(QT_DIR_PATH "$ENV{QTDIR}")
        if(QT_DIR_PATH STREQUAL "")
            message(STATUS "QTDIR environment variable not found")
        endif()
    endif()

    if(QT_DIR_PATH STREQUAL "")
        message(STATUS "QT not found, probably will fail now ... ")
    else()
        list(APPEND CMAKE_PREFIX_PATH "${QT_DIR_PATH}")
        message(STATUS "QT path found: ${QT_DIR_PATH}")
    endif()

    find_package(Qt6 COMPONENTS Core Widgets QUIET)
    if (NOT Qt6_FOUND)
        find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
        set(USING_QT 5)
        message(STATUS "Using QT ${Qt5_VERSION}")
    else()
        message(STATUS "Using QT ${Qt6_VERSION}")
        set(USING_QT 6)
    endif()
endmacro()


macro(check_platform_and_compiler)
    # TODO: cross-compilation ???
    if (WIN32)
        message(STATUS "Compiling operator-app for windows")

        if (!MINGW)
            # make sure that you are using mingw toolset
            # Add ${QT_DIR}/bin and ${QT_DIR}/plugins/platforms to PATH (solves dll not found problem)
            # ? QMAKE_LFLAGS +=-static-libgcc -static-libstdc++
            # TODO: manage toolsets; mingw81_64 just one possible option
            message(WARNING "Compiler is not MinGW; Going to fail ...")
        endif()
    else()
        message(STATUS "Compiling operator-app for linux")
    endif()
endmacro()
