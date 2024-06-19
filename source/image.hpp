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
        bool valid() {return pixels == nullptr ? false : true;}
        // Loads an image from disk.
        void load(const char* path) {pixels = stbi_load(path, &width, &height, &this->channels, 4);}
        // Creats an ImGui draw call for the image (Requires loadOnGPU()). 
        void ImGuiDraw(int width, int height) {ImGui::Image((void*)(intptr_t)textureID, ImVec2(width, height));}

        // Set the filter modes for the GPU texture (Requires loadOnGPU()).
        void minMagFilter(unsigned int min, unsigned int mag) {
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
        }
        // Set the wrapping mode for the GPU texture (Requires loadOnGPU()).
        void wrappingMode(unsigned int S, unsigned int T) {
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T);
        }
        // Loads the image on to the GPU.
        void loadOnGPU(unsigned int min, unsigned int mag, unsigned int S, unsigned int T) {
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        }
        // Unloads the image off GPU texture.
        void unloadOffGPU() {glDeleteTextures(1, &textureID);}

        // Returns the GPU texture ID (0 is none).
        int getTextureID() {return textureID;}
        // Returns the width of the image.
        int getWidth() {return width;}
        // Returns the height of the image.
        int getHeight() {return height;}
        // Returns the pixel data for the image (DO NOT MODIFY).
        unsigned char* getPixels() {return pixels;}

        image() {
            textureID = 0, width = 0, height = 0, channels = 0;
            pixels = nullptr;
        }
        image(const char* path) {load(path);}
        ~image() {
            unloadOffGPU();
            stbi_image_free(pixels);
        }
    };
}
