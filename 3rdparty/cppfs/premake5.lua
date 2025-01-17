print(os.date() .. " Prepare cppfs submodule...")

cppfs_config_file = io.open("./cppfs_api.h", "r")
cppfs_config = cppfs_config_file:read("*a")
cppfs_config_file:close()

cppfs_config_file = io.open("./cppfs/source/cppfs/include/cppfs/cppfs_api.h", "w")
cppfs_config_file:write(cppfs_config)
cppfs_config_file:close()

project "cppfs"
    kind "StaticLib"
    language "C++"
    cppdialect "C++14"
    characterset "MBCS"
    location "./"
    staticruntime "on"
    rtti("on")

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./cppfs/source/cppfs/include/*.h",
        "./cppfs/source/cppfs/source/*cpp"
    }

    includedirs
    {
        "./cppfs/source/cppfs/include",
        "./cppfs/source/cppfs/3rdparty/"
    }

    defines
    {
        "CPPFS_STATIC_DEFINE",
        "_SCL_SECURE_NO_WARNINGS",
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "./cppfs/source/cppfs/include/windows/*.h",
            "./cppfs/source/cppfs/source/windows/*.cpp"
        }

        defines
        {
            "SYSTEM_WINDOWS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"