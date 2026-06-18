project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "../QuadArchEngine/src",
        "../vendor/glfw/include",
        "../vendor/glad"
    }

    links {
        "QuadArchEngine"
    }

    filter "system:windows"
        systemversion "latest"