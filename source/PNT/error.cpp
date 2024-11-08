#include <PNT/error.hpp>

#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

namespace PNT {
#ifndef PNT_NO_CONSOLE_LOG
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("pentagramLogs/", 0, 0, true);
    std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};
    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("Pentagram log", sinks.begin(), sinks.end());
#endif
#ifdef PNT_NO_CONSOLE_LOG
    auto fileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("pentagramLogs/", 0, 0, true);
    std::vector<spdlog::sink_ptr> sinks{fileSink};
    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("Pentagram log", sinks.begin(), sinks.end());
#endif
}

auto userConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
auto userFileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/", 0, 0, true);
std::vector<spdlog::sink_ptr> userSinks{userConsoleSink, userFileSink};
#ifndef PNT_USER_LOGGER_NAME
std::shared_ptr<spdlog::logger> userLogger = std::make_shared<spdlog::logger>("Log", userSinks.begin(), userSinks.end());
#endif
#ifdef PNT_USER_LOGGER_NAME
std::shared_ptr<spdlog::logger> userLogger = std::make_shared<spdlog::logger>(PNT_USER_LOGGER_NAME, userSinks.begin(), userSinks.end());
#endif

void PNT::assertMsg(const char* file, int line, int code) {
    std::string buffer;
    switch(code) {
        case 0:
            buffer = "[PNT]Pentagram not initialized yet";
            break;
        case 1:
            buffer = "[PNT]Pentagram already initialized";
            break;
        case 2:
            buffer = "[PNT]Window null or not created yet";
            break;
        case 3:
            buffer = "[PNT]Window not null or already created";
            break;
        case 4:
            buffer = "[PNT]Newframe required";
            break;
        case 5:
            buffer = "[PNT]Endframe required";
            break;
        case 6:
            buffer = "[PNT]Shader ID is 0 (0 indicates not created yet)";
            break;
        case 7:
            buffer = "[PNT]Shader ID is not 0 (0 indicates not created yet)";
            break;
        case 8:
            buffer = "[PNT]Program ID is 0 (0 indicates not created yet)";
            break;
        case 9:
            buffer = "[PNT]Program ID is not 0 (0 indicates not created yet)";
            break;
    }
    logger.get()->error("{}, this occured at: {}:{}", buffer, file, line);
    exit(EXIT_FAILURE);
}

void PNT::errorCallback(int errorCode, const char* errorDescription) {
    logger.get()->error("[PNTinternal] !PNT CORE ERROR! GLFW has crashed with code: {}, and message: {}", errorCode, errorDescription);
    exit(EXIT_FAILURE);
}
