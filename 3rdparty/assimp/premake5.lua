print(os.date() .. " Prepare assimp submodule...")
print(os.date() .. " Building assimp with CMake...")

assimp_config_file = io.open("./config.h", "r")
assimp_config = assimp_config_file:read("*a")
assimp_config_file:close()

assimp_config_file = io.open("./assimp/include/assimp/config.h", "w")
assimp_config_file:write(assimp_config)
assimp_config_file:close()

module = "assimp"
if not isdir("./lib") then
    cmake = "cmake"
    os.execute("{MKDIR} lib")
    os.execute("{MKDIR} bin")

    os.execute(cmake .. " -S ./" .. module .. " -B ./" .. module .. "/auto_build")

    print("BUILD DEBUG VERSION OF" .. module)
    os.execute(cmake .. " --build ./" .. module .. "/auto_build --config Debug")
    os.execute(cmake .. " --build ./" .. module .. "/auto_build --config Release")

    os.execute("{MOVE} ./" .. module .. "/auto_build/lib/Debug ./lib")
    os.execute("{MOVE} ./" .. module .. "/auto_build/bin/Debug ./bin")
    os.execute("{MOVE} ./" .. module .. "/auto_build/lib/Release ./lib")
    os.execute("{MOVE} ./" .. module .. "/auto_build/bin/Release ./bin")

end