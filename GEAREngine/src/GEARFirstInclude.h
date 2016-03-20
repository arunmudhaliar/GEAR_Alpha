#pragma once

#ifdef _WIN32
    //define something for Windows (32-bit and 64-bit, this part is common)
    #define GEAR_WINDOWS    1
    #define GEAR_EDITOR 1
    #ifdef _WIN64
        //define something for Windows (64-bit only)
        #define GEAR_WIN64  1
    #else
        #define GEAR_WIN32  1
    #endif
#elif __APPLE__
    #include "TargetConditionals.h"

    #define GEAR_APPLE    1
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
        #define GEAR_IOS_SIMULATOR    1
        #define GEAR_IOS    1
    #elif TARGET_OS_IPHONE
        // iOS device
        #define GEAR_IOS_DEVICE    1
        #define GEAR_IOS    1
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
        #define GEAR_MACOS    1
        #define GEAR_EDITOR 1
    #else
        #   error "Unknown Apple platform"
    #endif
#elif ANDROID
    #define GEAR_ANDROID    1
//#elif __linux__
//    // linux
//#elif __unix__ // all unices not caught above
//    // Unix
//#elif defined(_POSIX_VERSION)
//    // POSIX
#else
    #error "Unknown platform"
#endif



//For debugging
#if defined(GEAR_WINDOWS)
    #if defined(_DEBUG)
        #define GEAR_DEBUG  1
    #endif
#elif defined(GEAR_APPLE)
    #if defined(DEBUG)
        #define GEAR_DEBUG  1
    #endif
#elif defined(GEAR_ANDROID)
    //TODO: Need to confirm which flag is used for debug in android.
#endif