#include <error.hpp>

#include <iostream>
#include <spdlog/spdlog.h>

void PNT::requireWindow(const char *file, int line) {
    spdlog::error("[PNT]Window null or not created yet, this occured at: {}:{}", file, line);
    exit(EXIT_FAILURE);
}

void PNT::requireNoWindow(const char *file, int line) {
    spdlog::error("[PNT]Window not null or already created, this occured at: {}:{}", file, line);
    exit(EXIT_FAILURE);
}

void PNT::requireNewfrme(const char *file, int line) {
    spdlog::error("[PNT]Newframe required, this occured at: {}:{}", file, line);
    exit(EXIT_FAILURE);
}

void PNT::requireEndfrme(const char *file, int line) {
    spdlog::error("[PNT]Endframe required, this occured at: {}:{}", file, line);
    exit(EXIT_FAILURE);
}

void PNT::errorCallback(int errorCode, const char *errorDescription) {
    spdlog::error("[PNTinternal] !PNT CORE ERROR! GLFW has crashed with code: {}, and message: {}", errorCode, errorDescription);
    exit(EXIT_FAILURE);
}
