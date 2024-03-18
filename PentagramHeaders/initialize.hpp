#pragma once

#include <includes.hpp>
#include <logger.hpp>

namespace PNT
{
    void initialize(int windowWidth = 700, int windowHeight = 400, const char *windowTitle = "Window")
    {
        uint32_t windowFlags = (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

        log.sender = "SDL";
        log.log(1, "Initializing SDL... ");
        int SDLErrorCode = SDL_Init(SDL_INIT_VIDEO);
        if (SDLErrorCode != 0)
        {
            log.postfix("Failed");
            log.log(4, SDL_GetError());
        }
        log.postfix("Succeeded");

        const char *glsl_version = "#version 460";

        SDL_Window *window = SDL_CreateWindow(windowTitle, windowWidth, windowHeight, windowFlags);
        log.log(1, "Creating Window... ");
        if (window == NULL)
        {
            log.postfix("Failed");
            log.log(4, SDL_GetError());
        }
        log.postfix("Succeeded");
    }
}
