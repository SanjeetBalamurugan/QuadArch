workspace "QuadArch"
    architecture "x64"
    startproject "Sandbox"
    staticruntime "off"

    configurations { "Debug", "Release" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    -- Dependencies
    include "vendor/glfw"

    -- Projects
    include "QuadArchEngine"
    include "Sandbox"

newaction {
    trigger     = "clean",
    description = "Removes all binaries, intermediate build files, and generated project files",
    execute     = function()
        print("Cleaning project...")

        os.rmdir("bin")
        os.rmdir("bin-int")
        os.rmdir(".vs")

        os.rmdir("vendor/glfw/bin")
        os.rmdir("vendor/glfw/bin-int")

        local function cleanFiles(pattern)
            local files = os.matchfiles(pattern)
            for _, file in ipairs(files) do
                os.remove(file)
            end
        end

        cleanFiles("*.sln")
        cleanFiles("**/*.vcxproj")
        cleanFiles("**/*.vcxproj.filters")
        cleanFiles("**/*.vcxproj.user")
        cleanFiles("**/*.Makefile")
        cleanFiles("**/Makefile")

        print("Clean complete!")
    end
}