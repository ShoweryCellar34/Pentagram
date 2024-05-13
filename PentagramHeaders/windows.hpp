#pragma once

#include <includes.hpp>
#include <logger.hpp>
#include <enumerations.hpp>
#include <utilities/ptrToChar.hpp>

namespace PNT
{
    struct windowData
    {
        std::string title = "";
        short width = -1, height = -1;
        short x = -1, y = -1;
        char vsyncMode = -1;
        char visiblity = -1;
        float clearColor[4] = {-1.0f, -1.0f, -1.0f, -1.0f};
    };

    class Window
    {
    public:
        // Constructor/Destructor
        Window(const char *title = "Title", unsigned short width = 600, unsigned short height = 600, SDL_WindowFlags flags = SDL_WINDOW_OPENGL)
        {
            instances++;
            instanceList.push_back(this);

            data.title = (char *)title;
            data.width = width;
            data.height = height;

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            window = SDL_CreateWindow(data.title.c_str(), data.width, data.height, flags | SDL_WINDOW_OPENGL);
            windowID = SDL_GetWindowID(window);
            unsigned char currentDisplay = SDL_GetDisplayForWindow(window);
            SDL_SetWindowPosition(window, (SDL_GetCurrentDisplayMode(currentDisplay)->w / 2) - (data.width / 2), (SDL_GetCurrentDisplayMode(currentDisplay)->h / 2) - (data.height / 2) + 1);

            openglContext = SDL_GL_CreateContext(window);
            gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

            ImGuiContext = ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            ImGui_ImplSDL3_InitForOpenGL(window, openglContext);
            ImGui_ImplOpenGL3_Init(glsl_version);
            ImGui::StyleColorsDark();
        }
        ~Window()
        {
            auto instanceID = std::find(instanceList.begin(), instanceList.end(), this);
            instanceList.erase(instanceID);
            instances--;

            ImGui::SetCurrentContext(ImGuiContext);
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext(ImGuiContext);

            SDL_GL_DeleteContext(openglContext);

            SDL_DestroyWindow(window);
        }

        // Starts the opengl and imgui frame for the window, returns the sdl error code (0 is success)..
        int startFrame()
        {
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
                startFrameListener(this);
            }
            return errorCode;
        }

        // Hides the window, returns the sdl error code (0 is success). 
        int endFrame()
        {
            int errorCode = 0;
            ImGui::Render();
            ImGuiIO &io = ImGui::GetIO();
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            errorCode = SDL_GL_SwapWindow(window);
            ImGui::UpdatePlatformWindows();
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            if(endFrameListener != nullptr)
            {
                endFrameListener(this);
            }
            return errorCode;
        }

        // Processes the current event, listener functionality supported (check setlistener() function for details).
        void eventProcess()
        {
            if(event.window.windowID == windowID)
            {
                ImGui_ImplSDL3_ProcessEvent(&event);
                switch(event.window.type)
                {
                case SDL_EVENT_WINDOW_RESIZED:
                    data.width = event.window.data1;
                    data.height = event.window.data2;
                    break;

                case SDL_EVENT_WINDOW_SHOWN:
                    data.visiblity = true;
                    break;

                case SDL_EVENT_WINDOW_HIDDEN:
                    data.visiblity = false;
                    break;

                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    setVisiblity(false);
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
                    eventListener(this);
                }
            }
        }

        // Returns the data struct of the window.
        windowData getWindowData()
        {
            return data;
        }

        // Sets the data struct of the window.
        void setWindowData(windowData newData)
        {
            if(!newData.title.empty()) setTitle(newData.title.c_str());
            setDimentions(newData.width, newData.height);
            setPosition(newData.x, newData.y);
            if(newData.vsyncMode != -1) setVsyncMode(newData.vsyncMode);
            if(newData.visiblity != -1) setVisiblity(newData.visiblity);
            setClearColor(newData.clearColor[0], newData.clearColor[1], newData.clearColor[2], newData.clearColor[3]);
        }

        // Sets the title of the window, returns the sdl error code (0 is success).
        int setTitle(const char *newTitle)
        {
            int errorCode = 0;
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

        // Sets the visiblity of the window, returns the sdl error code (0 is success).
        int setVisiblity(bool newVisiblity)
        {
            int errorCode = 0;
            data.visiblity = newVisiblity;
            newVisiblity == true ? errorCode = SDL_RaiseWindow(window) : errorCode = SDL_HideWindow(window);
            if(errorCode != 0)
            {
                log.log(2, SDL_GetError());
            }
            return errorCode;
        }

        // Sets the vsync mode of the window (0 = off, 1 = on, -1 = adaptive), returns the sdl error code (0 is success).
        int setVsyncMode(char newVsyncMode)
        {
            int errorCode;
            data.vsyncMode = newVsyncMode;
            errorCode = SDL_GL_SetSwapInterval(newVsyncMode);
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

        // Sets the listener for the specified event (use nullptr to clear listener).
        void setListener(unsigned char listenerID, void (*newListener)(Window *))
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

            default:
                break;
            }
        }

    private:
        // SDL data
        SDL_Window *window;
        SDL_GLContext openglContext;

        // Window data
        unsigned char windowID;
        windowData data;

        // Event data
        static inline SDL_Event event = SDL_Event();
        friend SDL_Event getEvent();
        friend bool pollEvent();


        // ImGui data
        ImGuiContext *ImGuiContext;
        const char *glsl_version = "#version 460";

        // listener data
        void (*startFrameListener)(Window *) = nullptr;
        void (*endFrameListener)(Window *) = nullptr;
        void (*eventListener)(Window *) = nullptr;

        // other data
        static inline int instances;
        static inline std::vector<Window *> instanceList;
        char instanceID;
    };
}
