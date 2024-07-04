#include <init.hpp>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <window.hpp>

namespace PNT {
    // Init/deinit definitions.

    bool init() {
        return glfwInit();
    }

    void deinit() {
        for(const Window* window : Window::instancesList) {
            delete window;
        }
        glfwTerminate();
    }
}
