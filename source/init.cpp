#include <init.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <error.hpp>
#include <window.hpp>

namespace PNT {
    // Init/deinit definitions.

    bool init() {
        glfwSetErrorCallback();
        return glfwInit();
    }

    void deinit() {
        for(const Window* window : Window::instancesList) {
            delete window;
        }
        glfwTerminate();
    }
}
