print(os.date() .. " Prepare GLM submodule...")

if not isdir("./glm/include") then
    os.execute("mkdir \"./glm/include\"")
    os.execute("move ./glm/glm ./glm/include")
end