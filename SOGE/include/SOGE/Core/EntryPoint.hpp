#ifndef SOGE_CORE_ENTRYPOINT_HPP
#define SOGE_CORE_ENTRYPOINT_HPP

#include "SOGE/System/StringId.hpp"
#include "SOGE/Utils/Logger.hpp"


extern soge::Engine* soge::CreateApplication();

#ifdef SOGE_WINDOWS

#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nShowCmd)
{

#ifdef SOGE_DEBUG
    AllocConsole();
    freopen("CONOUT$", "w+", stdout);
#endif // SOGE_DEBUG

    soge::Logger::Init();

    // TODO remove after testing
    using namespace soge::string_id_literals;

    auto logStringId = [](const soge::StringId& id) {
        const auto cStr = id.GetString().data();
        SOGE_INFO_LOG("String identifier with value \"{}\" has hash {} and pointer {}", cStr, id.GetHash(),
                      static_cast<const void*>(cStr));
    };

    constexpr auto constId = "Hello, World"_sid;
    logStringId(constId);

    std::string string = "Hello, World";
    const auto allocatedId = soge::StringId(string);
    logStringId(allocatedId);
    assert(allocatedId == constId && "Compile-time and runtime StringIds should be equal");

    const auto reusedId = soge::StringId("Hello, World");
    logStringId(reusedId);
    assert(allocatedId.GetString().data() == allocatedId.GetString().data() &&
           "StringId should be able to reuse existing strings at runtime");

    string += "!";
    const auto newId = soge::StringId(string);
    logStringId(newId);
    assert(allocatedId != newId && "StringId should not reuse existing strings if they have been modified");

    const auto app = soge::CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#else

int main(int argc, char** argv)
{
    soge::Logger::Init();

    auto app = soge::CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif // SOGE_WINDOWS
#endif // SOGE_CORE_ENTRYPOINT_HPP
