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
            return visiblity;
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
            visiblity = true;
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
            visiblity = false;
            errorCode = SDL_HideWindow(window);
            if(errorCode != 0)
            {
                errorData.errorCode = errorCode;
                return errorData;
            }
        }

        void eventProcess()
        {
            if(event.window.windowID == windowID)
            {
                switch (event.window.type)
                {
                case SDL_EVENT_WINDOW_RESIZED:
                    width = event.window.data1;
                    height = event.window.data2;
                    break;

                case SDL_EVENT_WINDOW_SHOWN:
                    visiblity = true;
                    break;

                case SDL_EVENT_WINDOW_HIDDEN:
                    visiblity = false;
                    break;

                default:
                    break;
                }
            }
        }

        Window(const char *windowTitle = "Title", int windowWidth = 700, int windowHeight = 400, SDL_WindowFlags windowFlags = SDL_WINDOW_OPENGL)
        {
            instances++;
            errorData.errorSource = "SDL";
            ptrToChar(title, windowTitle);
            width = windowWidth;
            height = windowHeight;
            window = SDL_CreateWindow(title, width, height, windowFlags | SDL_WINDOW_OPENGL);
            windowID = SDL_GetWindowID(window);
        }
        ~Window()
        {
            instances--;
        }
    private:
        static unsigned short instances;

        const char *title = nullptr;
        unsigned short width = 0;
        unsigned short height = 0;
        bool visiblity = true;
        unsigned short windowID = 0;

        SDL_Window *window = nullptr;
        SDL_Event event;

        errorData errorData;
        int errorCode = 0;
    };
}
