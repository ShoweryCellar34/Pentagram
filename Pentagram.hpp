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

    // Fills a structure with time data,or if no structure specified, returns the data.
    void calculateTime(PENTA::time *inputTime)
    {
        time_t currentTime = std::time(0);
        std::tm* now = std::localtime(&currentTime);
        inputTime->second = std::to_string(now->tm_sec);
        inputTime->minute = std::to_string(now->tm_min);
        inputTime->hour = std::to_string(now->tm_hour);
        inputTime->day = std::to_string(now->tm_mday);
        inputTime->month = std::to_string(now->tm_mon + 1);
        inputTime->year = std::to_string(now->tm_year + 1900);
    }
}


class logger
{
public:
    void log(int level, std::string message)
    {
        calculateTime(&currentTime);
        buffer = '[' + numToStr(level) + "][" + currentTime.hour + ':' + currentTime.minute + ':' + currentTime.second + "]: " + message;
        std::cout << buffer << std::endl;
        myfile << buffer << std::endl;
    }

    logger()
    {
        calculateTime(&currentTime);
        myfile.open(currentTime.day + '-' + currentTime.month + '-' + currentTime.year + ".txt");
    }
private:
    PENTA::time currentTime;
    std::string buffer;
    std::ofstream myfile;

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
