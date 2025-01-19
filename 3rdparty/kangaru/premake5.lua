
kangaru_config_file = io.open("./config.hpp", "r")
kangaru_config = kangaru_config_file:read("*a")
kangaru_config_file:close()

kangaru_config_file = io.open("./kangaru/include/kangaru/detail/config.hpp", "w")
kangaru_config_file:write(kangaru_config)
kangaru_config_file:close()

project "kangaru"
    kind "None"
    language "C++"
    location "./"
    staticruntime "on"

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./kangaru/include/kangaru/**.hpp",
    }

    includedirs
    {
        "./kangaru/include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
