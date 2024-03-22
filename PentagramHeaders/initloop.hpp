#pragma once

#include <includes.hpp>
#include <logger.hpp>

namespace PNT
{
    struct errorData
    {
        std::string errorSource;
        int errorCode;
    };

    uint32_t windowFlags = 0;
    const char *glsl_version = "#version 460";
    SDL_Window *window = nullptr;
    SDL_GLContext openglContext = nullptr;
    ImGuiIO io;

    errorData initialize(unsigned short windowWidth = 700, unsigned short windowHeight = 400, const char *windowTitle = "Window", uint32_t windowsFlagsTemp = (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY))
    {
        errorData errorData;

        // Get the current source and save it for later
        const char *ptr = log.source.c_str();
        char oldSource[log.source.length() + 1];
        for (int i = 0; i < log.source.length(); i++) {
            oldSource[i] = ptr[i];
        }
        oldSource[log.source.length()] = '\0';

        // Windows Flags
        windowFlags = windowsFlagsTemp;

        // SDL Setup
        log.source = "SDL";
        log.log(0, "Initializing SDL... ");
        int errorCode = SDL_Init(SDL_INIT_VIDEO);
        if (errorCode != 0)
        {
            log.postfix("Failed");
            log.log(3, SDL_GetError());
            errorData.errorSource = log.source;
            errorData.errorCode = errorCode;
            return errorData;
        }
        log.postfix("Succeeded");

        window = SDL_CreateWindow(windowTitle, windowWidth, windowHeight, windowFlags);
        // Window Setup
        log.log(0, "Creating Window... ");
        if (window == NULL)
        {
            log.postfix("Failed");
            log.log(3, SDL_GetError());
            errorData.errorSource = log.source;
            errorData.errorCode = errorCode;
            return errorData;
        }
        log.postfix("Succeeded");

        // OpenGL Context Creation
        log.source = "SDL_GL";
        log.log(0, "Creating OpenGL Context... ");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        openglContext = SDL_GL_CreateContext(window);
        if(openglContext == NULL)
        {
            log.postfix("Failed");
            log.log(3, SDL_GetError());
            errorData.errorSource = log.source;
            errorData.errorCode = -1;
            return errorData;
        }
        log.postfix("Succeeded");

        // OpenGL Context Setup
        log.log(0, "Seting up OpenGL context... ");
        errorCode = SDL_GL_MakeCurrent(window, openglContext);
        if (errorCode != 0)
        {
            log.postfix("Failed");
            log.log(3, SDL_GetError());
            errorData.errorSource = log.source;
            errorData.errorCode = errorCode;
            return errorData;
        }
        log.postfix("Succeeded");

        // Setup GLAD
        log.source = "GLAD";
        log.log(0, "Initializing GLAD... ");
        errorCode = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
        if (!errorCode)
        {
            log.postfix("Failed");
            errorData.errorSource = log.source;
            errorData.errorCode = errorCode;
            return errorData;
        }
        log.postfix("Succeeded");

        // Setup ImGui
        log.source = "ImGui";
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        ImGui::StyleColorsDark();
        log.log(0, "Initializing Imgui For SDL2... ");
        errorCode = ImGui_ImplSDL3_InitForOpenGL(window, openglContext);
        if (errorCode == 0)
        {
            log.postfix("Failed");
            errorData.errorSource = log.source;
            errorData.errorCode = errorCode;
            return errorData;
        }
        log.postfix("Succeeded");
        log.log(0, "Initializing Imgui For OpenGL... ");
        errorCode = ImGui_ImplOpenGL3_Init(glsl_version);
        if (errorCode == 0)
        {
            log.postfix("Failed");
            errorData.errorSource = log.source;
            errorData.errorCode = errorCode;
            return errorData;
        }
        log.postfix("Succeeded");

        log.source = oldSource;
        return errorData;
    }

    SDL_Event startFrame(bool *running)
    {
        glClearColor(255, 255, 255, 255);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        return event;
    }

    errorData endFrame()
    {
        errorData errorData;
        int errorCode;
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        errorCode = SDL_GL_SwapWindow(window);
        if(errorCode != 0)
        {
            log.log(3, "Failed to swap framebuffers");
            log.log(3, SDL_GetError());
            errorData.errorSource = "SDL_GL";
            errorData.errorCode = errorCode;
        }
        return errorData;
    }
}
