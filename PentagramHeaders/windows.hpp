#pragma once

#include <includes.hpp>
#include <enumerations.hpp>
#include <utilities/ptrToChar.hpp>

namespace PNT
{
    struct windowData
    {
        unsigned char windowID;
        char *title;
        unsigned short width, height;
        unsigned short x, y;
        char vsyncMode;
        bool hidden;
        float clearColor[4];
    };

    class Window
    {
    private:
        // SDL data
        SDL_Window *window;
        SDL_GLContext openglContext;

        // Window data
        windowData data;

        // Universal window event data
        static inline SDL_Event event = SDL_Event();

        // ImGui data
        ImGuiContext *ImGuiContext;
        ImGuiIO io;
        const char *glsl_version = "#version 460";

        // listener data
        void (*startFrameListener)();
        void (*endFrameListener)();
        void (*eventListener)();
        void (*errorListener)();

        // other data
        long startTime;
        long endTime;
        double deltaTime;
        static inline int instances;
    public:
        // Returns the data struct of the window.
        windowData getWindowData()
        {
            return data;
        }

        // Sets the data struct of the window.
        void setWindowData(windowData newData)
        {
            data = newData;
        }

        // Sets the title of the window, returns the sdl error code (0 is success).
        int setTitle(const char *newTitle)
        {
            window;
            int errorCode = 0;
            ptrToChar(data.title, newTitle);
            errorCode = SDL_SetWindowTitle(window, newTitle);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
        }

        // Sets the width and height of the window (-1 = unchanged), returns the sdl error code (0 is success).
        int setDimentions(unsigned short newWidth, unsigned short newHeight)
        {
            int errorCode = 0;
            errorCode = SDL_SetWindowSize(window, newWidth == -1 ? data.width : newWidth, newHeight == -1 ? data.height : newHeight);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
        }

        // Sets the x and y coordinates of the window (-1 = unchanged), returns the sdl error code (0 is success).
        int setPosition(int newX, int newY)
        {
            int errorCode = 0;
            errorCode = SDL_SetWindowPosition(window, newX == -1 ? data.x : newX, newY == -1 ? data.y : newY);
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
        int vsync(char newVsyncMode)
        {
            int errorCode;
            data.vsyncMode = newVsyncMode;
            errorCode = SDL_GL_SetSwapInterval(data.vsyncMode);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
        }

        // Sets the opengl clear color for the window (-1 = unchanged).
        void setClearColor(float red = -1, float green = -1, float blue = -1, float alpha = -1)
        {
            if(red != -1) data.clearColor[0] = red;
            if(green != -1) data.clearColor[1] = green;
            if(blue != -1) data.clearColor[2] = blue;
            if(alpha != -1) data.clearColor[3] = alpha;
        }

        // Starts the opengl and imgui frames for the window, returns the sdl error code (0 is success)..
        int startFrame()
        {
            startTime = std::chrono::high_resolution_clock::now();
            int errorCode = 0;
            errorCode = SDL_GL_MakeCurrent(window, openglContext);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
                return errorCode;
            }
            glClearColor(data.clearColor[0], data.clearColor[1], data.clearColor[2], data.clearColor[3]);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui::SetCurrentContext(ImGuiContext);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
            if(startFrameListener != nullptr)
            {
                startFrameListener();
            }
            return errorCode;
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
            if(endFrameListener != nullptr)
            {
                endFrameListener();
            }
            endTime = std::chrono::high_resolution_clock::now();
            deltaTime = (endTime - startTime) * 1000;
            return errorCode;
        }

        // Sets the listener for the specified event (use nullptr to clear listener).
        void setListener(unsigned char listenerID, void (*newListener)())
        {
            switch(listenerID)
            {
            case PNT_LISTENER_FLAGS_EVENT:
                eventListener = newListener;
                break;

        	case PNT_LISTENER_FLAGS_STARTFRAME:
                startFrameListener = newListener;
                break;

            case PNT_LISTENER_FLAGS_ENDFRAME:
                endFrameListener = newListener;
                break;

            case PNT_LISTENER_FLAGS_ERROR:
                errorListener = newListener;
                break;

            default:
                break;
            }
        }

        /* Processes the current event, listener functionality supported (check setEventlistener() function for details),
        takes a boolean as a parameter setting it to true if a close request was detected for the window.*/
        void eventProcess(bool *shouldClose)
        {
            if(event.window.windowID == data.windowID)
            {
                ImGui_ImplSDL3_ProcessEvent(&event);
                switch(event.window.type)
                {
                case SDL_EVENT_WINDOW_RESIZED:
                    data.width = event.window.data1;
                    data.height = event.window.data2;
                    break;

                case SDL_EVENT_WINDOW_SHOWN:
                    data.hidden = true;
                    break;

                case SDL_EVENT_WINDOW_HIDDEN:
                    data.hidden = false;
                    break;

                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    *shouldClose = true;
                    break;

                case SDL_EVENT_WINDOW_MOVED:
                    data.x = event.window.data1;
                    data.y = event.window.data2;
                    break;

                default:
                    break;
                }
                if(eventListener != nullptr)
                {
                    eventListener();
                }
            }
        }

        // Constructor/Destructor
        Window(const char *title = "Title", unsigned short width = 600, unsigned short height = 600, SDL_WindowFlags flags = SDL_WINDOW_OPENGL)
        {
            instances++;
            data.title = (char *)title;
            data.width = width;
            data.height = height;
            data.vsyncMode = false;
            data.hidden = false;
            data.clearColor = {1.0f, 1.0f, 1.0f, 1.0f};

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            window = SDL_CreateWindow(data.title, data.width, data.height, flags | SDL_WINDOW_OPENGL);
            data.windowID = SDL_GetWindowID(window);
            unsigned char currentDisplay = SDL_GetDisplayForWindow(window);
            SDL_SetWindowPosition(window, (SDL_GetCurrentDisplayMode(currentDisplay)->w / 2) - (data.width / 2), (SDL_GetCurrentDisplayMode(currentDisplay)->h / 2) - (data.height / 2) + 1);

            openglContext = SDL_GL_CreateContext(window);
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
            delete[] data.title;
        }
    };
}
