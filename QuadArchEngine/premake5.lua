project "QuadArchEngine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "../vendor/glfw/include",
        "../vendor/glad"
    }

    defines {
        "QUAD_BUILD_DLL"
    }

    links {
        "GLFW"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "on"

        defines {
            "_WINDLL"
        }