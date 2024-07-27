#include <init.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <error.hpp>
#include <window.hpp>

namespace PNT {
    extern std::shared_ptr<spdlog::logger> logger;

    // Init/deinit definitions.

    bool init() {
        logger.get()->set_pattern("%d-%m-%Y %H:%M:%S %l : %v", spdlog::pattern_time_type::utc);
        logger.get()->flush_on(spdlog::level::trace);
        logger.get()->info("[PNT]Initializing Pentagram");
        glfwSetErrorCallback(errorCallback);
        return glfwInit();
    }

    void deinit() {
        logger.get()->info("[PNT]Shutting down Pentagram");
        spdlog::shutdown();
        for(Window* window : Window::instancesList) {
            window->destroyWindow();
        }
        glfwTerminate();
    }
}
