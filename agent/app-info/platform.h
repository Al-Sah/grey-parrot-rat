//
// Created by alsah on 12.03.23.
//

#ifndef GREY_PARROT_AGENT_PLATFORM_H
#define GREY_PARROT_AGENT_PLATFORM_H

/**
 * Determination a platform of an operation system
 * Based on https://sourceforge.net/p/predef/wiki/OperatingSystems/
 */
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
    // Most used Macro
    #define PLATFORM_NAME "Windows"
    #define PLATFORM_IS_WINDOWS
#elif defined(__ANDROID__)
    // Android (implies Linux, so it must come first)
    #define PLATFORM_NAME "undefined"
    #error "plarform is not supported"
#elif defined(__linux__) || defined(__gnu_linux__)
    // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
    #define PLATFORM_NAME "Linux"
    #define PLATFORM_IS_LINUX
#else
    #define PLATFORM_NAME "undefined"
    #error "plarform is not supported"
#endif


/**
 * Platform architecture determination
 * https://sourceforge.net/p/predef/wiki/Architectures/
 */
#include <cstdint>

#if INTPTR_MAX == INT32_MAX
    #define PLATFORM_ARCHITECTURE 32
#elif INTPTR_MAX == INT64_MAX
    #define PLATFORM_ARCHITECTURE 64
#else
    #define PLATFORM_ARCHITECTURE 0
#endif

#endif //GREY_PARROT_AGENT_PLATFORM_H
