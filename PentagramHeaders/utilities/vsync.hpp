#pragma once
#define VSYNC

#include <includes.hpp>

namespace PNT
{
    // Toggles vsync (some systems support adaptive vsync), returns an error data struct.
    errorData vsync(int mode)
    {
        errorData errorData;
        int errorCode;
        log.log(0, "Configuring Vsync... ", "SDL_GL");
        errorCode = SDL_GL_SetSwapInterval(mode);
        if(errorCode == -1)
        {
            log.postfix("Failed");
            log.log(2, SDL_GetError());
            errorData.source = log.source;
            errorData.code = errorCode;
            return errorData;
        }
        log.postfix("Succeeded");
        return errorData;
    }
}
