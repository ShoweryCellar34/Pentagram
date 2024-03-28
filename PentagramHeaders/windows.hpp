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

        // Sets the window name.
        void setTitle(char *newTitle)
        {
            ptrToChar(title, newTitle);
            SDL_SetWindowTitle(window, newTitle);
        }

        Window(char *windowTitle = "Title", int windowWidth = 700, int windowHeight = 400)
        {
            instances++;
            ptrToChar(title, windowTitle);
            width = windowWidth;
            height = windowHeight;
            SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
        }
        ~Window()
        {
            instances--;
        }
    private:
        static unsigned short instances;

        const char *title = nullptr;
        int width = 0;
        int height = 0;

        SDL_Window *window = nullptr;
    };
}
