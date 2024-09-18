#ifndef SOGE_ENTRY_POINT_HPP
#define SOGE_ENTRY_POINT_HPP

#include "SOGE/Utils/Logger.hpp"


extern soge::Engine* soge::CreateApplication();

int main(int argc, char** argv)
{
    soge::Logger::Init();

    auto app = soge::CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif // !SOGE_ENTRY_POINT_HPP
