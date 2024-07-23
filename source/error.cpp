#include <error.hpp>

namespace PNT {
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("Pentagram console log");
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
    }
    logger.get()->error(buffer, file, line);
    exit(EXIT_FAILURE);
}

void PNT::errorCallback(int errorCode, const char *errorDescription) {
    logger.get()->error("[PNTinternal] !PNT CORE ERROR! GLFW has crashed with code: {}, and message: {}", errorCode, errorDescription);
    exit(EXIT_FAILURE);
}
