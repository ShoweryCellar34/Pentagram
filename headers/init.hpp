#pragma once

#include <GLFW/glfw3.h>

namespace PNT {
    /// @brief Starts Pentagram.
    /// @return True if startup was succesful and false if there was an error.
    bool init();

    /// @brief Shutsdown Pentagram (All windows are deleted and handles become invalid).
    void deinit();
}
