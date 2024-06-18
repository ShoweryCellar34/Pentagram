#pragma once

#include <includes.hpp>

namespace PNT {
    class image {
    private:
        unsigned int textureID;
        int width, height, channels;
        unsigned char* pixels;

    public:
        // Returns true if image not null (null meaning invalid), and false if image is null.
        bool valid() {return pixels == nullptr ? 0 : 1;}
        // Loads an image from disk.
        void load(const char* path, int channels) {pixels = stbi_load(path, &width, &height, &this->channels, channels);}
        // Creats an ImGui draw call for the image (Requires loadOnGPU() to be called). 
        void ImGuiDraw(int width, int height) {ImGui::Image((void*)(intptr_t)textureID, ImVec2(width, height));}
        // Loads the image on to the GPU.
        void loadOnGPU() {
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        }
        // Unloads the image off GPU texture.
        void unloadOnGPU() {glDeleteTextures(1, &textureID);}

        // Returns the GPU texture ID (0 is none).
        int getTextureID() {return textureID;}
        // Returns the width of the image.
        int getWidth() {return width;}
        // Returns the height of the image.
        int getHeight() {return height;}
        // Returns the pixel data for the image (NOT A COPY).
        unsigned char* getPixels() {return pixels;}

        image() {
            width = 0, height = 0, channels = 0;
            pixels = nullptr;
        }
        image(const char* path, short channels) {load(path, channels);}
        ~image() {stbi_image_free(pixels);}
    };
}
