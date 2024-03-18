#pragma once

#include <includes.hpp>

namespace PNT
{
    struct timeData
    {
        std::string second, minute, hour, day, month, year;
    };

    // Fills a structure with time data, or if no structure specified, returns the data.
    void calculateTime(timeData *inputTime)
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

    class logger
    {
    public:
        std::string sender;

        void log(int level, const char *message)
        {
            calculateTime(&currentTime);
            buffer = ('[' + currentTime.hour + ':' + currentTime.minute + ':' + currentTime.second + "][" + numToStr(level) + (sender.empty() ? "" : "][" + sender) + "]: " + message);
            std::cout << std::endl << buffer;
            myfile << std::endl << buffer;
        }
        void log(int level, const char *message, std::string sender)
        {
            calculateTime(&currentTime);
            sender = "][" + sender;
            buffer = ('[' + currentTime.hour + ':' + currentTime.minute + ':' + currentTime.second + "][" + numToStr(level) + sender + "]: " + message);
            std::cout << std::endl << buffer;
            myfile << buffer << std::endl;
        }
        void postfix(const char *postfix)
        {
            buffer = postfix;
            std::cout << buffer;
            myfile << buffer;
        }

        logger()
        {
            calculateTime(&currentTime);
            myfile.open(currentTime.day + '-' + currentTime.month + '-' + currentTime.year + ".txt");
        }
    private:
        timeData currentTime;
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

    logger log;
}
