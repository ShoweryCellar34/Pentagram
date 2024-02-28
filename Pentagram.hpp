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

enum loggerFlags {
    NOTE = 1,
    WARNING = 2,
    ERROR = 3,
    FATAL = 4
};

class logger
{
public:

    void log(int level, std::string message)
    {
        std::string buffer;
    }
};
