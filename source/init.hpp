#pragma once

#include "includes.hpp"
#include "windows.hpp"

namespace PNT {

    /// @brief Starts Pentagram.
    void init() {glfwInit();}

    /// @brief Shutsdown Pentagram (All windows are deleted and handles become invalid).
    void deinit() {glfwTerminate();}
}
