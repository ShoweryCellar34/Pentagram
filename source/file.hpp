#pragma once

#include "includes.hpp"

namespace PNT {
    /// @brief Returns the contents of the specified file.
    /// @param filePath Path of the desired file to read.
    /// @return The file contents.
    std::string fileToString(const char* filePath) {
        std::ifstream file(filePath);
        std::string output;
        std::string line;

        while(getline(file, line)) {
            output.append(line + "\n");
        }

        file.close();
        return output;
    }
}
