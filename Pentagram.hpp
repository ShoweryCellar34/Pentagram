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
#include <bitset>

enum loggerFlags
{
    NOTE = 1,
    WARNING = 2,
    ERROR = 3,
    FATAL = 4
};

enum timeFlags
{
    SECOND = 1,
    MINUTE = 2,
    HOUR = 4,
    DAY = 8,
    MONTH = 16,
    YEAR = 32,
    ALL = 64
};

class logger
{
public:
    void log(int level, std::string message)
    {
        std::string buffer = '[' + numToStr(level) + "][" + getTime(ALL, true) + "]: " + message;
        std::cout << buffer << std::endl;
        std::string logName = getTime(DAY | MONTH | YEAR, true);
        myfile.open (logName + ".txt");
        myfile << "Writing this to a file.\n";
        myfile.close();
    }
private:
    std::ofstream myfile;
    char buffer[80];
    std::string getTime(int components, bool filler)
    {
        std::bitset<7> flags(components);
        std::string format;
        if(flags.test(6))
        {
            format.append("%d-%m-%Y %H:%M:%S");
        }
        else
        {
            if(flags.test(3))
            {
                if(flags.test(4) & filler)
                {
                    format.append("%d-");
                }
                else
                {
                    format.append("%d");
                }
            }
            if(flags.test(4))
            {
                if(flags.test(5) & filler)
                {
                    format.append("%m-");
                }
                else
                {
                    format.append("%m");
                }
            }
            if(flags.test(5))
            {
                if(flags.test(2) & filler)
                {
                    format.append("%Y ");
                }
                else
                {
                    format.append("%Y");
                }
            }
            if(flags.test(2))
            {
                if(flags.test(1) & filler)
                {
                    format.append("%H:");
                }
                else
                {
                    format.append("%H");
                }
            }
            if(flags.test(1))
            {
                if(flags.test(0) & filler)
                {
                    format.append("%M:");
                }
                else
                {
                    format.append("%M");
                }
            }
            if(flags.test(0))
            {
                format.append("%S");
            }
        }
        time_t currentTime = time(0);
        strftime(buffer, 80, format.c_str(), localtime(&currentTime));
        return buffer;
    }

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
