#include <init.hpp>
#include <GLFW/glfw3.h>

namespace PNT {
    bool init() {
        return glfwInit();
    }

    void deinit() {
        glfwTerminate();
    }
}
