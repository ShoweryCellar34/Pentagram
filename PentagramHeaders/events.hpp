#pragma once

#include <includes.hpp>

namespace PNT {
    // returns the universal event for all windows.
    void pollEvents() {
        glfwPollEvents();
    }
}
