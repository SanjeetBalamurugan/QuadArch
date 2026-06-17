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
        "../vendor/glfw/include"
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

        postbuildcommands {
            "{COPYFILE} \"%{cfg.targetdir}/%{cfg.targetname}%{cfg.targetext}\" \"../bin/" .. outputdir .. "/Sandbox\""
        }