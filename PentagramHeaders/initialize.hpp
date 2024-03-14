#pragma once

#include <includes.hpp>
#include <logger.hpp>

namespace PNT
{
    int initialize()
    {
        log.log(1, "Initializing SDL...");
        int SDLErrorCode = 0;
        SDLErrorCode = SDL_Init(SDL_INIT_VIDEO);
        if (SDLErrorCode != 0)
        {
            log.log(4, SDL_GetError());
            return SDLErrorCode;
        }
        log.log(1, "Initialized SDL");
        return 0;
    }
}
