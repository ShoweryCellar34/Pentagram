#pragma once

#include <cstdint>
#include <imgui.h>
#include <glad.h>

namespace PNT {
    class image {
    private:
        int width = 0, height = 0, channels = 0;
        unsigned char* pixels = nullptr;
        unsigned int textureID = 0;

    public:
        image();

        /// @brief Image object constructor.
        /// @param path The desired image path.
        image(const char* path);

        image(const image& original);

        ~image();

        /// @brief Checks the state of the image.
        /// @return The state of the image, true for valid and false for invalid (null).
        bool valid() const;

        /// @brief Loads an image from disk.
        /// @param path Image path on disk.
        void load(const char* path);

        /// @brief Creats an ImGui draw call for the image (Requires loadOnGPU()).
        /// @param dimensions The gui image element size.
        void ImGuiDraw(ImVec2 dimentions) const;

        /// @brief Set the settings for the GPU texture (Requires loadOnGPU()).
        /// @param min Texture filtering for minification.
        /// @param mag Texture filtering for magmification.
        /// @param S Horizontal wrapping mode.
        /// @param T Vertical wrapping mode.
        void imageSettings(unsigned int min, unsigned int mag, unsigned int S, unsigned int T);

        /// @brief Loads the image on to the GPU with settings.
        /// @param min Texture filtering for minification.
        /// @param mag Texture filtering for magmification.
        /// @param S Horizontal wrapping mode.
        /// @param T Vertical wrapping mode.
        void loadOnGPU(unsigned int min = GL_NEAREST_MIPMAP_NEAREST, unsigned int mag = GL_NEAREST_MIPMAP_NEAREST, unsigned int S = GL_CLAMP_TO_EDGE, unsigned int T = GL_CLAMP_TO_EDGE);

        /// @brief Unloads the texture off GPU.
        void unloadOffGPU();

        /// @brief Gets the texture ID.
        /// @return Returns the GPU texture ID (0 means not on GPU).
        int getTextureID() const;

        /// @brief Get the image width.
        /// @return Returns the width of the image.
        uint32_t getWidth() const;

        /// @brief Get the image height.
        /// @return Returns the height of the image.
        uint32_t getHeight() const;

        /// @brief Gets the image pixels.
        /// @return Returns the pixel data for the image (DO NOT MODIFY).
        unsigned char* getPixels() const;
    };
}
