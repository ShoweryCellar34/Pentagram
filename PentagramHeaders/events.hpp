#pragma once

#include <includes.hpp>
#include <windows.hpp>

namespace PNT
{
    // returns the universal event for all windows.
    SDL_Event getEvent()
    {
        return Window::event;
    }

    // Polls the event (should be called in the while loop).
    bool pollEvent()
    {
        return SDL_PollEvent(&Window::event);
    }
}
