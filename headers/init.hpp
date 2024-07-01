#pragma once

#include "includes.hpp"

namespace PNT {
    /// @brief Starts Pentagram.
    /// @return True if startup was succesful and false if there was an error.
    bool init();
    /// @brief Shutsdown Pentagram (Breaks all windows).
    void deinit();
}
