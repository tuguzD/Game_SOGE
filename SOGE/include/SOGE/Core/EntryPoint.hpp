#ifndef SOGE_ENTRY_POINT_HPP
#define SOGE_ENTRY_POINT_HPP

#include "SOGE/Utils/Logger.hpp"


extern soge::Engine* soge::CreateApplication();

#ifdef SOGE_WINDOWS

#include <Windows.h>

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow
)
{

#ifdef SOGE_DEBUG
    AllocConsole();
    freopen("CONOUT$", "w+", stdout);
#endif // SOGE_DEBUG

    soge::Logger::Init();

    auto app = soge::CreateApplication();
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
#endif // !SOGE_ENTRY_POINT_HPP
