#ifndef SOGE_EVENT_SDL_EVENTSSDL_HPP
#define SOGE_EVENT_SDL_EVENTSSDL_HPP

#include "SOGE/Core/Event/Event.hpp"
#include <SDL3/SDL_events.h>


namespace soge
{
    namespace EventTypes
    {
        namespace SDLEvents
        {
            constexpr EventType g_SDLKeyDownEvent{StringId("SDLKeyDownEvent"), EventCategory::Input};
            constexpr EventType g_SDLKeyUpEvent{StringId("SDLKeyUpEvent"), EventCategory::Input};
            constexpr EventType g_SDLTextEditingEvent{StringId("SDLTextEditing"), EventCategory::Input};
            constexpr EventType g_SDLTextInputEvent{StringId("SDLTextInput"), EventCategory::Input};
            constexpr EventType g_SDLKeyMapChangedEvent{StringId("SDLKeyMapChanged"), EventCategory::Input};

            constexpr EventType g_SDLKeyboardAddedEvent{StringId("SDLKeyboardAdded"), EventCategory::Input};
            constexpr EventType g_SDLKeyboardRemovedEvent{StringId("SDLKeyboardRemoved"), EventCategory::Input};
        }
    }

    class SDLEvent : public StaticEvent<SDLEvent>
    {
    private:
        SDL_Event* m_sdlEvent;

    public:


    };

}

#endif // !SOGE_EVENT_SDL_EVENTSSDL_HPP
