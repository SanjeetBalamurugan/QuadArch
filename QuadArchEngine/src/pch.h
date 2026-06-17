#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <KHR/khrplatform.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif