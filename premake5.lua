include "Premake5/scripts/helpers.lua"
include "dependencies.lua"

print(os.date() .. " Initialize submodules...")
os.execute("git submodule update --init --recursive")

workspace "SOGE"
    architecture "x64"
    startproject "GAME"
    configurations {"Debug", "Release"}

    buildpattern = "%{cfg.buildcfg}.%{cfg.system}.%{cfg.architecture}"

    project "SOGE"
        location "SOGE"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"

        targetdir("build/bin/" .. buildpattern .. "/%{prj.name}")
        objdir("build/int/" .. buildpattern .. "/%{prj.name}")

        pchheader "sogepch.hpp"
        pchsource "%{wks.location}/%{prj.name}/source/sogepch.cpp"

        files
        {
            "%{wks.location}/%{prj.name}/include/**.hpp",
            "%{wks.location}/%{prj.name}/source/**.cpp"
        }

        includedirs
        {
            "%{wks.location}/%{prj.name}/include",
            "%{wks.location}/%{IncludeThirdpartyDirs.spdlog}"
        }

        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "SPDLOG_WCHAR_TO_UTF8_SUPPORT"
        }

        links
        {

        }

        filter "system:windows"
            systemversion "latest"

            defines
            {
                "SOGE_WINDOWS"
            }

        filter "configurations:Debug"
            symbols "on"

            defines
            {
                "SOGE_DEBUG",
                "SOGE_ENABLE_ASSERT"
            }

            links {}

        filter "configurations:Release"
            optimize "on"

            defines
            {
                "SOGE_RELEASE"
            }

            links {}

-----------------------
--- Engine application
-----------------------

    project "GAME"
        location "GAME"
        -- kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"

        -- Executable will be placed in the root of application folder
        -- to have easier eaccess to game resources.
        targetdir("%{wks.location}/%{prj.name}/")
        objdir("build/int/" .. buildpattern .. "/%{prj.name}")

        files
        {
            "%{wks.location}/%{prj.name}/include/**.hpp",
            "%{wks.location}/%{prj.name}/source/**.cpp"
        }

        includedirs
        {
            "%{wks.location}/%{prj.name}/include",
            "%{wks.location}/SOGE/include",
            "%{wks.location}/%{IncludeThirdpartyDirs.spdlog}"
        }

        links
        {
            "SOGE"
        }

        filter "system:windows"
            systemversion "latest"
            kind 'WindowedApp'
            entrypoint "wWinMainCRTStartup"

            defines
            {
                "SOGE_WINDOWS"
            }

        filter "configurations:Debug"
            symbols "on"

            defines
            {
                "SOGE_DEBUG"
            }

        filter "configurations:Release"
            optimize "on"

            defines
            {
                "SOGE_RELEASE"
            }
