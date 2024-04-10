#pragma once
#define VSYNC

#include <includes.hpp>

namespace PNT
{
    // Toggles vsync (some systems support adaptive vsync), returns an error data struct.
    int vsync(int mode)
    {
        int errorCode;
        errorCode = SDL_GL_SetSwapInterval(mode);
        if(errorCode != 0)
        {
            log.log(2, SDL_GetError());
        }
        return errorCode;
    }
}
