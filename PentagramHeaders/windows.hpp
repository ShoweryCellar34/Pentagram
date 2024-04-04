#pragma once

#include <includes.hpp>
#include <logger.hpp>
#include <utilities/ptrToChar.hpp>

namespace PNT
{
    class Window
    {
    public:
        // Returns the window title.
        const char *getTitle()
        {
            return title;
        }
        // Returns the width and height.
        unsigned short getDimentions()
        {
            return width, height;
        }
        bool getVisiblity()
        {
            return visible;
        }

        // Sets the window name.
        errorData setTitle(const char *newTitle)
        {
            ptrToChar(title, newTitle);
            errorCode = SDL_SetWindowTitle(window, newTitle);
            if(errorCode != 0)
            {
                errorData.errorCode = errorCode;
                return errorData;
            }
        }
        // Sets the window width and height.
        errorData setDimentions(unsigned short newWidth, unsigned short newHeight)
        {
            width = newWidth;
            height = newHeight;
            errorCode = SDL_SetWindowSize(window, newWidth, newHeight);
            if(errorCode != 0)
            {
                errorData.errorCode = errorCode;
                return errorData;
            }
        }
        // Shows the window.
        errorData show()
        {
            visible = true;
            errorCode = SDL_RaiseWindow(window);
            if(errorCode != 0)
            {
                errorData.errorCode = errorCode;
                return errorData;
            }
        }
        // Hides the window.
        errorData hide()
        {
            visible = false;
            errorCode = SDL_HideWindow(window);
            if(errorCode != 0)
            {
                errorData.errorCode = errorCode;
                return errorData;
            }
        }

        void startFrame(unsigned short red = 255, unsigned short green = 255, unsigned short blue = 255, unsigned short alpha = 255)
        {
            glClearColor((float)red/255, (float)green/255, (float)blue/255, (float)alpha/255);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        SDL_Event eventProcess(bool *open = nullptr)
        {
            if(event.window.windowID == windowID)
            {
                if(SDL_PollEvent(&event))
                {
                    switch(event.window.type)
                    {
                    case SDL_EVENT_WINDOW_RESIZED:
                        width = event.window.data1;
                        height = event.window.data2;
                        break;

                    case SDL_EVENT_WINDOW_SHOWN:
                        visible = true;
                        break;

                    case SDL_EVENT_WINDOW_HIDDEN:
                        visible = false;
                        break;

                    case SDL_EVENT_QUIT:
                        *open = false;
                        break;

                    default:
                        return event; 
                        break;
                    }
                }
            }
            return SDL_Event();
        }
        void endFrame()
        {
            SDL_GL_SwapWindow(window);
        }

        Window(const char *windowTitle = "Title", int windowWidth = 700, int windowHeight = 400, SDL_WindowFlags windowFlags = SDL_WINDOW_OPENGL)
        {
            errorData.errorSource = "SDL";
            ptrToChar(title, windowTitle);
            width = windowWidth;
            height = windowHeight;
            window = SDL_CreateWindow(title, width, height, windowFlags | SDL_WINDOW_OPENGL);
            windowID = SDL_GetWindowID(window);
            openglContext = SDL_GL_CreateContext(window);
            SDL_GL_MakeCurrent(window, openglContext);
            gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
        }
        ~Window()
        {
            SDL_DestroyWindow(window);
            SDL_GL_DeleteContext(openglContext);
        }
    private:
        char *title = new char[0];
        unsigned short width = 0;
        unsigned short height = 0;
        bool visible = true;
        unsigned short windowID = 0;

        SDL_Window *window = nullptr;
        SDL_Event event;
        SDL_GLContext openglContext;



        errorData errorData;
        int errorCode = 0;
    };
}
