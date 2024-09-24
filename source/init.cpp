#include <init.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <error.hpp>
#include <window.hpp>

namespace PNT {
    bool initialized = false;
    extern std::shared_ptr<spdlog::logger> logger;

    // Init/deinit definitions.

    bool init() {
        PNT_REQUIRE_DEINIT();

        initialized = glfwInit();
        logger.get()->set_pattern("%d-%m-%Y %H:%M:%S %l : %v", spdlog::pattern_time_type::utc);
        logger.get()->flush_on(spdlog::level::trace);
        logger.get()->info("[PNT]Initializing Pentagram");
        glfwSetErrorCallback(errorCallback);
        return initialized;
    }

    void deinit() {
        PNT_REQUIRE_INIT();

        logger.get()->info("[PNT]Shutting down Pentagram");
        for(Window* window : Window::m_instancesList) {
            window->destroyWindow();
        }
        spdlog::shutdown();
        glfwTerminate();
        initialized = false;
    }
}
