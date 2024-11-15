#include <PNT/init.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <PNT/error.hpp>
#include <PNT/window.hpp>

namespace PNT {
    bool initialized = false;
    extern std::shared_ptr<spdlog::logger> logger;

    // Init/deinit definitions.

    bool init() {
        initialized = glfwInit();
        logger.get()->set_pattern("%d-%m-%Y %H:%M:%S %l : %v", spdlog::pattern_time_type::utc);
        logger.get()->flush_on(spdlog::level::trace);
        logger.get()->info("[PNT]Initializing Pentagram");
        glfwSetErrorCallback(errorCallback);
        glfwSetMonitorCallback(monitorCallback);
        return initialized;
    }

    void deinit() {
        logger.get()->info("[PNT]Shutting down Pentagram");
        for(Window* window : Window::m_instancesList) {
            window->destroyWindow();
        }
        spdlog::shutdown();
        glfwTerminate();
        initialized = false;
    }
}
