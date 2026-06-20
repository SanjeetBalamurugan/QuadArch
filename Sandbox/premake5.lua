project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "../QuadArchEngine/src",
        "../vendor/glfw/include",
        "../vendor/glm/"
    }

    links {
        "QuadArchEngine"
    }

    filter "system:windows"
        systemversion "latest"