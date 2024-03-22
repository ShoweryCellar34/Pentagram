#pragma once

#include <includes.hpp>

namespace PNT
{
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
            errorData.errorSource = log.source;
            errorData.errorCode = errorCode;
            return errorData;
        }
        log.postfix("Succeeded");
        return errorData;
    }
}
