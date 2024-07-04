#pragma once

namespace PNT {
    /// @brief Starts Pentagram.
    /// @return True if startup was succesful and false if there was an error.
    bool init();

    /// @brief Shutsdown Pentagram (All windows are deleted and handles become invalid).
    void deinit();
}
