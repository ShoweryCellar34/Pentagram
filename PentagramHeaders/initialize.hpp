#pragma once

#include <includes.hpp>
#include <logger.hpp>

namespace PNT
{
    void initialize(int windowWidth = 700, int windowHeight = 400, const char *windowTitle = "Window")
    {
        // Windows Flags
        uint32_t windowFlags = (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

        // SDL Setup
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

        // Window Setup
        SDL_Window *window = SDL_CreateWindow(windowTitle, windowWidth, windowHeight, windowFlags);
        log.log(1, "Creating Window... ");
        if (window == NULL)
        {
            log.postfix("Failed");
            log.log(4, SDL_GetError());
        }
        log.postfix("Succeeded");

        // OpenGL Context Creation
        log.log(1, "Creating OpenGL Context... ");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GLContext openglContext = SDL_GL_CreateContext(window);
        if (openglContext == NULL)
        {
            log.postfix("Failed");
            log.log(4, SDL_GetError());
        }
        log.postfix("Succeeded");

        // OpenGL Context Setup
        log.log(1, "Seting up OpenGL context");
        SDLErrorCode = SDL_GL_MakeCurrent(window, openglContext);
        if (SDLErrorCode != 0)
        {
            log.FatalError("SDL Failed to Setup OpenGL Context: " + (std::string)SDL_GetError());
            log.FatalError("Exiting With Code " + std::to_string(SDLErrorCode));
            return SDLErrorCode;
        }
    }
}
