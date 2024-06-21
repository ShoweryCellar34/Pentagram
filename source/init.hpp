#pragma once

#include "includes.hpp"
#include "windows.hpp"

namespace PNT {
    /// @brief Starts Pentagram.
    /// @return True if startup was succesful and false if there was an error.
    bool init() {
        return glfwInit();
    }

    /// @brief Shutsdown Pentagram (All windows are deleted and handles become invalid).
    void deinit() {
        glfwTerminate();
    }
}
