#include <GLAD/gl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <imgui.h>
#include <cpp/imgui_stdlib.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include <SDL.h>

#include <stb_image.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

enum loggerFlags {
    NOTE = 1,
    WARNING = 2,
    ERROR = 3,
    FATAL = 4
};

class logger
{
private:
    char buffer[80];
    std::string getTime()
    {
        time_t currentTime = time(0);
        strftime(buffer, 80, "%d-%m-%Y %H:%M:%S", localtime(&currentTime));
        return buffer;
    }

    std::string numToStr(int number)
    {
    }
public:

    void log(int level, std::string message)
    {
        std::string buffer = '[' +  + "][" + getTime() + ']';
    }
};
