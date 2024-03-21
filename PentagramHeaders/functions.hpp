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

    std::optional<errorData> initialize(unsigned short windowWidth = 700, unsigned short windowHeight = 400, const char *windowTitle = "Window")
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
        uint32_t windowFlags = (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

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

        const char *glsl_version = "#version 460";

        // Window Setup
        SDL_Window *window = SDL_CreateWindow(windowTitle, windowWidth, windowHeight, windowFlags);
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
        SDL_GLContext openglContext = SDL_GL_CreateContext(window);
        if (openglContext == NULL)
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
        ImGuiIO &io = ImGui::GetIO();
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
    }

    std::optional<errorData> vsync(int mode)
    {
        errorData errorData;
        int errorCode;
        log.log(0, "Configuring Vsync... ", "SDL_GL");
        if (SDL_GL_SetSwapInterval(mode) == -1)
        {
            log.postfix("Failed");
            log.log(2, SDL_GetError());
            errorData.errorSource = log.source;
            errorData.errorCode = errorCode;
            return errorData;
        }
        log.postfix("Succeeded");
    }

    struct timeData
    {
        std::string second, minute, hour, day, month, year;
    };

    // Fills a structure with time data, or if no structure specified, returns the data.
    void calculateTime(timeData *inputTime)
    {
        time_t currentTime = std::time(0);
        std::tm* now = std::localtime(&currentTime);
        inputTime->second = std::to_string(now->tm_sec);
        inputTime->minute = std::to_string(now->tm_min);
        inputTime->hour = std::to_string(now->tm_hour);
        inputTime->day = std::to_string(now->tm_mday);
        inputTime->month = std::to_string(now->tm_mon + 1);
        inputTime->year = std::to_string(now->tm_year + 1900);
    }
}
