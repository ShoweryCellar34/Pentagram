#pragma once

#include "includes.hpp"
#include "windows.hpp"

namespace PNT {
    // initulizes Pentagram.
    void init() {glfwInit();}

    // shutsdown Pentagram (All windows are deleted and become invalid).
    void deinit() {glfwTerminate();}
}
