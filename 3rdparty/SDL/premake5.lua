project "SDL3"
    kind "StaticLib"
    location "./"
    staticruntime "on"

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./SDL/include/SDL3/*.h",
        "./SDL/src/**.h",
        "./SDL/src/**.c"
    }