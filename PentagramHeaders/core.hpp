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

    // Initialize SDL, GLAD and ImGui. Returns an error data struct.
    errorData initialize(unsigned short windowWidth = 700, unsigned short windowHeight = 400, const char *windowTitle = "Window", uint32_t windowsFlagsTemp = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY)
    {
        errorData errorData;
        int errorCode = 0;

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
        errorCode = SDL_Init(SDL_INIT_VIDEO);
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

    // Quits everything. Returns an error data struct.
    errorData deinitialize()
    {
        errorData errorData;
        int errorCode = 0;

        log.log(0, "Quitting ImGui... ", "ImGui");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        log.postfix("Succeeded");
        log.log(0, "Deleting the OpenGL Context... ", "SDL_GL");
        errorCode = SDL_GL_DeleteContext(openglContext);
        if(errorCode != 0)
        {
            log.postfix("Failed");
            log.log(2, SDL_GetError(), "SDL_GL");
            errorData.errorCode = errorCode;
            errorData.errorSource = "SDL_GL";
            return errorData;
        }
        log.postfix("Succeeded");
        log.log(0, "Destroying Window... ", "SDL");
        SDL_DestroyWindow(window);
        log.postfix("Succeeded");
        log.log(0, "Quitting SDL... ", "SDL");
        SDL_Quit();
        log.postfix("Succeeded");
        return errorData;
    }

    // Calls glClearColor, starts the ImGui frame and returns the Event (Closing the window is automaticly handled).
    SDL_Event startFrame(bool *running, unsigned short red = 255, unsigned short green = 255, unsigned short blue = 255, unsigned short alpha = 255)
    {
        glClearColor((float)red/255, (float)green/255, (float)blue/255, (float)alpha/255);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if(event.type == SDL_EVENT_QUIT)
            {
                *running = false;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        return event;
    }

    // Renders ImGui and swaps the framebuffers, returns an error data struct.
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
