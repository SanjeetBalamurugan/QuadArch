#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <utility>
#include <random>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <future>

#include <glad/glad.h>
#include <KHR/khrplatform.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif