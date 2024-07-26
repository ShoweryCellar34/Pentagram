#include <error.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

namespace PNT {
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/", 0, 0, true);
    std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};
    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("Pentagram log", sinks.begin(), sinks.end());
}

void PNT::assertMsg(const char *file, int line, int code) {
    char buffer[256] = {0};
    switch(code) {
    case 0:
        strcpy(buffer, "[PNT]Window null or not created yet, this occured at: {}:{}");
        break;

    case 1:
        strcpy(buffer, "[PNT]Window not null or already created, this occured at: {}:{}");
        break;

    case 2:
        strcpy(buffer, "[PNT]Newframe required, this occured at: {}:{}");
        break;

    case 3:
        strcpy(buffer, "[PNT]Endframe required, this occured at: {}:{}");
        break;

    case 4:
        strcpy(buffer, "[PNT]Shader ID is 0 (0 indicates not created yet), this occured at: {}:{}");
        break;

    case 5:
        strcpy(buffer, "[PNT]Shader ID is not 0 (0 indicates not created yet), this occured at: {}:{}");
        break;

    case 6:
        strcpy(buffer, "[PNT]Program ID is 0 (0 indicates not created yet), this occured at: {}:{}");
        break;

    case 7:
        strcpy(buffer, "[PNT]Program ID is not 0 (0 indicates not created yet), this occured at: {}:{}");
        break;
    }
    logger.get()->error(buffer, file, line);
    exit(EXIT_FAILURE);
}

void PNT::errorCallback(int errorCode, const char *errorDescription) {
    logger.get()->error("[PNTinternal] !PNT CORE ERROR! GLFW has crashed with code: {}, and message: {}", errorCode, errorDescription);
    exit(EXIT_FAILURE);
}
