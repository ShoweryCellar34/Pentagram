#pragma once

#include <includes.hpp>

namespace PNT {
    // Processes all pending events.
    void processEvents() {
        glfwPollEvents();
    }
}
