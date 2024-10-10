
-- Preparing EASTL directory
print(os.date() .. " Prepare EASTL submodule...")
os.execute("git clone https://github.com/electronicarts/EABase.git EASTL/deps/EABase")

project "EASTL"
    kind "StaticLib"
    language "C++"
    location "./"
    staticruntime "on"

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./EASTL/include/EASTL/**.h",
        "./EASTL/include/deps/EABase/include/Common/EABase**.h",
        "EASTL/source/**.cpp"
    }

    includedirs
    {
        "./EASTL/include",
        "./EASTL/deps/EABase/include/Common"
    }

    defines
    {
        "_CHAR16T",
        "_CRT_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_WARNINGS"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

