#pragma once

#include <includes.hpp>

namespace PNT {
    // initulizes GLFW
    void init() {
        glfwInit();
    }

    // deinitulizes GLFW
    void deinit() {
        glfwTerminate();
    }
}
