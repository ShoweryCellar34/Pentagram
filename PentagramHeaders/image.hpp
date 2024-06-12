#pragma once

#include <includes.hpp>

namespace PNT {
    struct image {
        int width, height, channels;
        unsigned char* pixels;

        bool valid() {return pixels == nullptr ? 0 : 1;}
        void setData(const char* path) {pixels = stbi_load(path, &width, &height, &channels, 0);}

        image() {
            width = 0, height = 0, channels = 0;
            pixels = nullptr;
        }
        image(const char* path) {setData(path);}
    };
}
