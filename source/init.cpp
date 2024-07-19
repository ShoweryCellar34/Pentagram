#include <init.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <error.hpp>
#include <window.hpp>

namespace PNT {
    // Init/deinit definitions.

    bool init() {
        glfwSetErrorCallback(errorCallback);
        return glfwInit();
    }

    void deinit() {
        for(Window* window : Window::instancesList) {
            window->destroyWindow();
        }
        glfwTerminate();
    }
}
