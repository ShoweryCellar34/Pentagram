#pragma once

#include "includes.hpp"

namespace PNT {
    // Returns the contents of the specified file.
    std::string fileToString(std::string filePath) {
        std::string output;
        std::string line;
        std::ifstream file(filePath);

        while(getline(file, line)) {output.append(line + "\n");}

        file.close();
        return output;
    }
}
