#pragma once

#include <includes.hpp>
#include <logger.hpp>
#include <utilities/ptrToChar.hpp>

namespace PNT
{
    class Window
    {
    public:
        static inline SDL_Event event = SDL_Event();
        // Returns the window title (WARN: returned value must be deleted using 'delete[] ...' when your done with it).
        char *getTitle()
        {
            char *arrayTitle = new char[0];
            ptrToChar(arrayTitle, title);
            return arrayTitle;
        }
        // Returns the width and height of the window.
        unsigned short getDimentions()
        {
            return width, height;
        }
        // Returns the position of the window.
        unsigned short getPosition()
        {
            return x, y;
        }
        // Returns the visiblity of the window.
        bool getVisiblity()
        {
            return hidden;
        }
        int getID()
        {
            return windowID;
        }

        // Sets the window name.
        int setTitle(const char *newTitle)
        {
            int errorCode = 0;
            ptrToChar(title, newTitle);
            errorCode = SDL_SetWindowTitle(window, newTitle);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
        }
        // Sets the width and height of the window, returns the sdl error code (0 is success).
        int setDimentions(unsigned short newWidth, unsigned short newHeight)
        {
            int errorCode = 0;
            errorCode = SDL_SetWindowSize(window, newWidth, newHeight);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
        }
        // Sets the x and y coordinates of the window, returns the sdl error code (0 is success).
        int setPosition(unsigned short newX, unsigned short newY)
        {
            int errorCode = 0;
            errorCode = SDL_SetWindowPosition(window, newX, newY);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
        }
        // Shows the window, returns the sdl error code (0 is success).
        int show()
        {
            int errorCode = 0;
            errorCode = SDL_RaiseWindow(window);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
        }
        // Hides the window, returns the sdl error code (0 is success).
        int hide()
        {
            int errorCode = 0;
            errorCode = SDL_HideWindow(window);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
        }

        void startFrame(unsigned short red = 255, unsigned short green = 255, unsigned short blue = 255, unsigned short alpha = 255)
        {
            SDL_GL_MakeCurrent(window, openglContext);
            glClearColor((float)red/255, (float)green/255, (float)blue/255, (float)alpha/255);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui::SetCurrentContext(ImGuiContext);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
        }

        // Sets the event callback to be called when a event for the current window takes place.
        void setEventCallback(void (*newEventCallback)(SDL_Event event))
        {
            void (*eventCallback)(SDL_Event event) = newEventCallback;
        }

        // Processes the current event callback functionality supported (Check setEventCallback() function for details), returns a boolean stating if the window should close.
        void eventProcess(bool *shouldClose = nullptr)
        {
            if(event.window.windowID == windowID)
            {
                ImGui_ImplSDL3_ProcessEvent(&event);
                switch(event.window.type)
                {
                case SDL_EVENT_WINDOW_RESIZED:
                    width = event.window.data1;
                    height = event.window.data2;
                    break;

                case SDL_EVENT_WINDOW_SHOWN:
                    hidden = true;
                    break;

                case SDL_EVENT_WINDOW_HIDDEN:
                    hidden = false;
                    break;

                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    *shouldClose = true;
                    break;

                case SDL_EVENT_WINDOW_MOVED:
                    x = event.window.data1;
                    y = event.window.data2;
                    break;

                default:
                    break;
                }
                if(eventCallback != nullptr)
                {
                    eventCallback(event);
                }
                *shouldClose = false;
            }
        }
        void endFrame()
        {
            ImGui::Render();
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
        }

        Window(const char *windowTitle = "Title", int windowWidth = 700, int windowHeight = 400, SDL_WindowFlags windowFlags = SDL_WINDOW_OPENGL)
        {
            instances++;
            ptrToChar(title, windowTitle);
            width = windowWidth;
            height = windowHeight;
            window = SDL_CreateWindow(title, width, height, windowFlags | SDL_WINDOW_OPENGL);
            windowID = SDL_GetWindowID(window);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            openglContext = SDL_GL_CreateContext(window);
            SDL_GL_MakeCurrent(window, openglContext);
            gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
            ImGuiContext = ImGui::CreateContext();
            ImGui::SetCurrentContext(ImGuiContext);
            ImGui_ImplSDL3_InitForOpenGL(window, openglContext);
            ImGui_ImplOpenGL3_Init(glsl_version);
            io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            ImGui::StyleColorsDark();
        }
        ~Window()
        {
            SDL_DestroyWindow(window);
            SDL_GL_DeleteContext(openglContext);
            ImGui::DestroyContext(ImGuiContext);
            delete title;
        }
    private:
        static inline int instances;

        char *title = new char[0];
        unsigned short width = 0, height = 0;
        unsigned short x, y = 0;
        bool hidden = true;
        unsigned short windowID = 0;

        SDL_Window *window = nullptr;
        SDL_GLContext openglContext = nullptr;

        ImGuiContext *ImGuiContext = nullptr;
        ImGuiIO io;
        const char *glsl_version = "#version 460";

        void (*eventCallback)(SDL_Event event) = nullptr;
    };
}
