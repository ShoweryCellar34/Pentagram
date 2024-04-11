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

        // Getters
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

        // Returns the window ID.
        int getID()
        {
            return windowID;
        }

        // Setters
        // Sets the title of the window, returns the sdl error code (0 is success).
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

        // Sets the vsync mode of the window (0 = off, 1 = on, -1 = adaptive), returns the sdl error code (0 is success).
        int vsync(short mode)
        {
            int errorCode;
            errorCode = SDL_GL_SetSwapInterval(mode);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
        }

        // Sets the opengl clear color for the window (-1 = unchanged).
        void setClearColor(float red = -1, float green = -1, float blue = -1, float alpha = -1)
        {
            if(red != -1)
            {
                rgba[0] = red;
            }
            if(green != -1)
            {
                rgba[1] = green;
            }
            if(blue != -1)
            {
                rgba[2] = blue;
            }
            if(alpha != -1)
            {
                rgba[3] = alpha;
            }
        }

        // Sets the callback for window events.
        void setEventCallback(void (*newEventCallback)(SDL_Event event))
        {
            eventCallback = newEventCallback;
        }

        // Starts the opengl and imgui frames for the window, returns the sdl error code (0 is success)..
        int startFrame()
        {
            int errorCode = 0;
            errorCode = SDL_GL_MakeCurrent(window, openglContext);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
                return errorCode;
            }
            glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui::SetCurrentContext(ImGuiContext);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
            return errorCode;
        }

        /* Processes the current event, callback functionality supported (Check setEventCallback() function for details),
        takes a boolean as a parameter setting it to true if a close request was detected for the window.*/
        void eventProcess(bool &shouldClose)
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
                    shouldClose = true;
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
            }
        }

        // Hides the window, returns the sdl error code (0 is success). 
        int endFrame()
        {
            int errorCode = 0;
            ImGui::Render();
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            errorCode = SDL_GL_SwapWindow(window);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
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
            ImGui::SetCurrentContext(ImGuiContext);
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext(ImGuiContext);
            SDL_GL_DeleteContext(openglContext);
            SDL_DestroyWindow(window);
            delete title;
        }
        void (*eventCallback)(SDL_Event event);
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

        float rgba[4] = {255.0f,  255.0f, 255.0f, 255.0f};
    };
}
