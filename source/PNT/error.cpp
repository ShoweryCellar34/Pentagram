#include <PNT/error.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

namespace PNT {
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("pentagramLogs/", 0, 0, true);
    std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};
    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("Pentagram log", sinks.begin(), sinks.end());
}

auto userConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
auto userFileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/", 0, 0, true);
std::vector<spdlog::sink_ptr> userSinks{userConsoleSink, userFileSink};
std::shared_ptr<spdlog::logger> userLogger = std::make_shared<spdlog::logger>("Log", userSinks.begin(), userSinks.end());

void PNT::assertMsg(const char* file, int line, int code) {
    char buffer[256] = {0};
    switch(code) {
    case 0:
        strcpy(buffer, "[PNT]Pentagram not initialized yet");
        break;

    case 1:
        strcpy(buffer, "[PNT]Pentagram already initialized");
        break;

    case 2:
        strcpy(buffer, "[PNT]Window null or not created yet");
        break;

    case 3:
        strcpy(buffer, "[PNT]Window not null or already created");
        break;

    case 4:
        strcpy(buffer, "[PNT]Newframe required");
        break;

    case 5:
        strcpy(buffer, "[PNT]Endframe required");
        break;

    case 6:
        strcpy(buffer, "[PNT]Shader ID is 0 (0 indicates not created yet)");
        break;

    case 7:
        strcpy(buffer, "[PNT]Shader ID is not 0 (0 indicates not created yet)");
        break;

    case 8:
        strcpy(buffer, "[PNT]Program ID is 0 (0 indicates not created yet)");
        break;

    case 9:
        strcpy(buffer, "[PNT]Program ID is not 0 (0 indicates not created yet)");
        break;
    }
    logger.get()->error("{}, this occured at: {}:{}", buffer, file, line);
    exit(EXIT_FAILURE);
}

void PNT::errorCallback(int errorCode, const char* errorDescription) {
    logger.get()->error("[PNTinternal] !PNT CORE ERROR! GLFW has crashed with code: {}, and message: {}", errorCode, errorDescription);
    exit(EXIT_FAILURE);
}
