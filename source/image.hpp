#pragma once

#include "includes.hpp"

namespace PNT {
    struct image {
        int width, height, channels;
        unsigned char* pixels;

        bool valid() {return pixels == nullptr ? 0 : 1;}
        void setData(const char* path, int channels) {pixels = stbi_load(path, &width, &height, &this->channels, channels);}

        image() {
            width = 0, height = 0, channels = 0;
            pixels = nullptr;
        }
        image(const char* path, short channels) {setData(path, channels);}
    };
}
