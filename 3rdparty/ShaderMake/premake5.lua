print(os.date() .. " Prepare ShaderMake submodule...")


module = "ShaderMake"
if not isdir("./bin") then
    cmake = "cmake"
    os.execute("{MKDIR} bin")

    os.execute(cmake .. " -S ./" .. module .. " -B ./" .. module .. "/auto_build -DSHADERMAKE_SEARCH_FOR_COMPILERS=ON")
    os.execute(cmake .. " --build ./" .. module .. "/auto_build --config Release")
    os.execute("{MOVE} ./" .. module .. "/bin/Release/* ./bin")
end

local cmake_cache_file = io.open("./" .. module .. "/auto_build/CMakeCache.txt", "r")
local cmake_cache = cmake_cache_file:read("*a")
cmake_cache_file:close()

FXC_PATH = cmake_cache:match("FXC_PATH:FILEPATH=(.-)\n")
DXC_PATH = cmake_cache:match("DXC_PATH:FILEPATH=(.-)\n")
DXC_SPIRV_PATH = cmake_cache:match("DXC_SPIRV_PATH:FILEPATH=(.-)\n")

SHADERMAKE_PATH = path.getabsolute("./bin/ShaderMake")
if os.host() == "windows" then
    SHADERMAKE_PATH = path.appendExtension(SHADERMAKE_PATH, ".exe")
end
