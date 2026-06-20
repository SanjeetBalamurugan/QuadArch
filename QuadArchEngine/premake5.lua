project "QuadArchEngine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files {
        "src/**.h",
        "src/**.cpp",
        "src/**.c"
    }

    includedirs {
        "src",
        "../vendor/glfw/include",
        "../vendor/glad",
        "../vendor/glm/"
    }

    defines {
        "QUAD_BUILD_DLL"
    }

    links {
        "GLFW",
        "opengl32.lib",
        "dwmapi.lib"
    }

    filter "files:src/glad.c"
        flags { "NoPCH" }
    filter {}

    filter "system:windows"
        systemversion "latest"

        defines {
            "_WINDLL"
        }
        defines { "GLFW_INCLUDE_NONE" }