#pragma once

#include <includes.hpp>

namespace PNT {
    struct image {
        int width, height, channels;
        unsigned char* pixels;

        bool validate() {return pixels == nullptr ? 0 : 1;}

        image() {}
        image(const char* path) {pixels = stbi_load("sky.jpg", &width, &height, &channels, 0);}
    };
}
