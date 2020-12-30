//
// Created by Saadat Baig on 30.12.20.
//
#ifndef SPOOF_DYLIB_LOG_UTILS_H
#define SPOOF_DYLIB_LOG_UTILS_H


// --------------------*  imporst  *--------------------
#include <stdio.h>  // printf


// --------------------*  color defs  *--------------------
#define cRESET  "\x1B[0m"
#define cRED  "\x1B[31m"
#define cGREEN  "\x1B[32m"
#define cYELLOW  "\x1B[33m"
#define cBLUE  "\x1B[34m"


// --------------------*  log macros  *--------------------
#define LOG_INFO(...)                   \
    do {                                \
        printf(cBLUE "[spoof_dylib][*]: ");   \
        printf("" __VA_ARGS__);         \
        printf(cRESET "\n");            \
    } while (0)

#define LOG_SUCCESS(...)                \
    do {                                \
        printf(cGREEN "[spoof_dylib][+]: ");  \
        printf("" __VA_ARGS__);         \
        printf(cRESET "\n");            \
    } while (0)

#define LOG_ERROR(...)                  \
    do {                                \
        printf(cRED "[spoof_dylib][-]: ");    \
        printf("" __VA_ARGS__);         \
        printf(cRESET "\n");            \
    } while (0)

#define LOG_DEBUG(...)                      \
    do {                                    \
        printf(cYELLOW "[spoof_dylib][DEBUG]: "); \
        printf("" __VA_ARGS__);             \
        printf(cRESET "\n");                \
    } while (0)


#endif //SPOOF_DYLIB_LOG_UTILS_H
