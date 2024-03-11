#pragma once

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
#include <fstream>
#include <string>
#include <ctime>

enum loggerFlags
{
    NOTE = 1,
    WARNING = 2,
    ERROR = 3,
    FATAL = 4
};

namespace PENTA
{
    struct time
    {
        std::string second, minute, hour, day, month, year;
    };
}

void calculateTime(PENTA::time *inputTime)
{
    time_t currentTime = time(0);
    std::tm* now = std::localtime(&currentTime);
    inputTime->second = std::to_string(now->tm_sec);
    inputTime->minute = std::to_string(now->tm_min);
}

class logger
{
public:
    void log(int level, std::string message)
    {
        calculateTime(&currentTime);
        myfile.open (date.day + '-' + date.month + '-' + date.year + ".txt");
        std::string buffer = '[' + numToStr(level) + "][" + currentTime.hour + ':' + currentTime.minute + ':' + currentTime.second + "]: " + message;
        std::cout << buffer << std::endl;
    }

    logger()
    {
        calculateTime(&date);
        myfile.open (date.day + '-' + date.month + '-' + date.year + ".txt");
        myfile.close();
    }
private:
    std::ofstream myfile;
    PENTA::time date;
    PENTA::time currentTime;

    std::string numToStr(int number)
    {
        switch(number)
        {
            case 1:
                return "NOTE";
                break;
            case 2:
                return "WARN";
                break;
            case 3:
                return "ERROR";
                break;
            case 4:
                return "FATAL";
                break;
            default:
                return "N/A";
        }
    }
};
