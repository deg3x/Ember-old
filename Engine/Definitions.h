#pragma once

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
// Add apple OS X definitions here
#endif