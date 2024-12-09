#include "sogepch.hpp"

#include "SOGE/Window/Impl/SDL/SDLWindowCore.hpp"

#undef CreateWindow


namespace soge
{
    SDLWindowCore::SDLWindowCore(SDLContext&)
    {
    }

    UniquePtr<Window> SDLWindowCore::CreateWindow(const WindowDesc& aWindowDesc)
    {
        return CreateUnique<SDLWindow>(aWindowDesc);
    }
}
