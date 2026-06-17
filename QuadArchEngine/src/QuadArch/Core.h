#pragma once

#ifdef _WIN32
    #ifdef QUAD_BUILD_DLL
        #define QuadAPI __declspec(dllexport)
    #else
        #define QuadAPI __declspec(dllimport)
    #endif
#else
    #ifdef QUAD_BUILD_DLL
        #define QuadAPI __attribute__((visibility("default")))
    #else
        #define QuadAPI
    #endif
#endif