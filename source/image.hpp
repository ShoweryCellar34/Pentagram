#pragma once

#include <includes.hpp>

namespace PNT {
    class image {
    private:
        int width = 0, height = 0, channels = 0;
        unsigned char* pixels = nullptr;
        unsigned int textureID = 0;

    public:
        /// @brief Checks the state of the image.
        /// @return The state of the image, true for valid and false for invalid (null).
        bool valid() {return pixels == nullptr ? false : true;}

        /// @brief Loads an image from disk.
        /// @param path Image path on disk.
        void load(const char* path) {pixels = stbi_load(path, &width, &height, &this->channels, 4);}

        /// @brief Creats an ImGui draw call for the image (Requires loadOnGPU()).
        /// @param width The width of the imgui image element.
        /// @param height The height of the imgui image element.
        void ImGuiDraw(int width, int height) {ImGui::Image((void*)(intptr_t)textureID, ImVec2(width, height));}

        /// @brief Set the settings for the GPU texture (Requires loadOnGPU()).
        /// @param min Texture filtering for minification.
        /// @param mag Texture filtering for magmification.
        /// @param S Horizontal wrapping mode.
        /// @param T Vertical wrapping mode.
        void imageSettings(unsigned int min, unsigned int mag, unsigned int S, unsigned int T) {
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T);
        }

        /// @brief Loads the image on to the GPU with settings.
        /// @param min Texture filtering for minification.
        /// @param mag Texture filtering for magmification.
        /// @param S Horizontal wrapping mode.
        /// @param T Vertical wrapping mode.
        void loadOnGPU(unsigned int min = GL_NEAREST_MIPMAP_NEAREST, unsigned int mag = GL_NEAREST_MIPMAP_NEAREST, unsigned int S = GL_CLAMP_TO_EDGE, unsigned int T = GL_CLAMP_TO_EDGE) {
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        }

        /// @brief Unloads the texture off GPU.
        void unloadOffGPU() {glDeleteTextures(1, &textureID);}

        /// @brief Gets the texture ID.
        /// @return Returns the GPU texture ID (0 means not on GPU).
        int getTextureID() {return textureID;}

        /// @brief Get the image dimentions.
        /// @return Returns the width and height in their respective order.
        std::pair<int, int> getDimentions() {return std::make_pair(width, height);}
        // 

        /// @brief Gets the image pixels.
        /// @return Returns the pixel data for the image (DO NOT MODIFY).
        unsigned char* getPixels() {return pixels;}

        /// @brief Image object constructor.
        /// @param path Image path on disk.
        image() {}
        image(const char* path) {
            load(path);
        }
        image(image& original) {
            width = original.width;
            height = original.height;
            channels = original.channels;
            pixels = original.pixels;
            printf(":%s:\n", original.pixels);
            if(original.textureID) {loadOnGPU();}
        }
        ~image() {
            unloadOffGPU();
            stbi_image_free(pixels);
        }
    };
}
