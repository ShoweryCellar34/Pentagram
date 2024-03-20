#pragma once

#include <includes.hpp>
#include <logger.hpp>

namespace PNT
{
    void initialize(unsigned short windowWidth = 700, unsigned short windowHeight = 400, const char *windowTitle = "Window")
    {
        const char *oldSender = log.sender.c_str();

        // Windows Flags
        uint32_t windowFlags = (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

        // SDL Setup
        log.sender = "SDL";
        log.log(0, "Initializing SDL... ");
        int SDLErrorCode = SDL_Init(SDL_INIT_VIDEO);
        if (SDLErrorCode != 0)
        {
            log.postfix("Failed");
            log.log(3, SDL_GetError());
        }
        log.postfix("Succeeded");

        const char *glsl_version = "#version 460";

        // Window Setup
        SDL_Window *window = SDL_CreateWindow(windowTitle, windowWidth, windowHeight, windowFlags);
        log.log(0, "Creating Window... ");
        if (window == NULL)
        {
            log.postfix("Failed");
            log.log(3, SDL_GetError());
        }
        log.postfix("Succeeded");

        // OpenGL Context Creation
        log.sender = "SDL_GL";
        log.log(0, "Creating OpenGL Context... ");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GLContext openglContext = SDL_GL_CreateContext(window);
        if (openglContext == NULL)
        {
            log.postfix("Failed");
            log.log(3, SDL_GetError());
        }
        log.postfix("Succeeded");

        // OpenGL Context Setup
        log.log(0, "Seting up OpenGL context... ");
        SDLErrorCode = SDL_GL_MakeCurrent(window, openglContext);
        if (SDLErrorCode != 0)
        {
            log.postfix("Failed");
            log.log(3, SDL_GetError());
        }
        log.postfix("Succeeded");

        // Setup GLAD
        log.sender = "GLAD";
        log.log(0, "Initializing GLAD... ");
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            log.postfix("Failed");
        }
        log.postfix("Succeeded");

        // Setup ImGui
        log.sender = "ImGui";
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        ImGui::StyleColorsDark();
        log.log(0, "Initializing Imgui For SDL2... ");
        if (ImGui_ImplSDL3_InitForOpenGL(window, openglContext) == 0)
        {
            log.postfix("Failed");
        }
        log.postfix("Succeeded");
        log.log(0, "Initializing Imgui For OpenGL... ");
        if (ImGui_ImplOpenGL3_Init(glsl_version) == 0)
        {
            log.postfix("Failed");
        }
        log.postfix("Succeeded");

        log.sender = oldSender;
    }

    int vsync(int mode)
    {
        log.log(0, "Configuring Vsync... ", "SDL_GL");
        if (SDL_GL_SetSwapInterval(mode) == -1)
        {
            log.postfix("Failed");
            log.log(2, SDL_GetError());
            return -1;
        }
        log.postfix("Succeeded");
        return 0;
    }
}
