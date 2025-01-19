print(os.date() .. " Prepare SDL3 submodule...")
print(os.date() .. " Building SDL3 with CMake...")

module = "SDL"
if not isdir("./lib") then
    cmake = "cmake"
    os.execute("mkdir lib")

    os.execute(cmake .. " -S ./" .. module .. " -B ./" .. module .. "/auto_build")

    print("BUILD DEBUG VERSION OF" .. module)
    os.execute(cmake .. " --build ./" .. module .. "/auto_build --config Debug")
    os.execute(cmake .. " --build ./" .. module .. "/auto_build --config Release")

    os.execute("move ./" .. module .. "/auto_build/Release ./lib")
    os.execute("move ./" .. module .. "/auto_build/Debug ./lib")

end