#pragma once

#define real float

#if defined(_WIN32)
    #if defined(_MSC_VER)
        #pragma warning(disable: 4251) 
        #ifdef ENGINE_SHARED_LIB
            #define ENGINE_API __declspec(dllexport)
        #else
            #define ENGINE_API __declspec(dllimport)
        #endif
    #else
        #error Only Microsoft C++ compiler is supported at the moment.
    #endif
#elif __APPLE__
    #define ENGINE_API __attribute__((visibility("default")))
#endif

#if defined(_WIN32)
    #define DOUBLE_SMALL DBL_EPSILON
    #define FLOAT_SMALL  FTL_EPSILON
#elif __APPLE__
    #define DOUBLE_SMALL __DBL_EPSILON__
    #define FLOAT_SMALL  __FLT_EPSILON__
#endif
