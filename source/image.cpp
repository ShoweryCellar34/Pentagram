#include "image.hpp"

#include <memory>
#include <spdlog/spdlog.h>
#include <stb_image.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad.h>

namespace PNT {
    extern std::shared_ptr<spdlog::logger> logger;

    // Image definitions.

    image::image() : m_width(128), m_height(128), m_channels(0), m_pixels(nullptr), m_textureID(0) {
        logger.get()->info("[PNT]Creating image");
    };

    image::image(const char* path) : m_width(128), m_height(128), m_channels(0), m_pixels(nullptr), m_textureID(0) {
        logger.get()->info("[PNT]Creating image");

        load(path);
    }

    image::image(const image& original) : m_width(128), m_height(128), m_channels(0), m_pixels(nullptr), m_textureID(0) {
        logger.get()->debug("[PNT]Copying image with width: {} and height: {}", m_width, m_height);

        m_width = original.m_width;
        m_height = original.m_height;
        m_channels = original.m_channels;
        m_pixels = original.m_pixels;
        strcpy((char*)m_pixels, (const char*)original.m_pixels);
        if(original.m_textureID) {
            loadOnGPU();
        }
    }

    image::~image() {
        logger.get()->info("[PNT]Destroying image");

        if(m_textureID) {
            unloadOffGPU();
        }
        if(m_pixels) {
            stbi_image_free(m_pixels);
        }
    }

    void image::load(const char* path) {
        logger.get()->debug("[PNT]Loading image from path \"{}\"", path);

        if(m_pixels != nullptr) {
            stbi_image_free(m_pixels);
        }
        m_pixels = stbi_load(path, (int*)&m_width, (int*)&m_height, (int*)&this->m_channels, 4);
        if(m_pixels == nullptr) {
            logger.get()->warn("Failed to load image");
        }
    }

    void image::load(std::string path) {
        logger.get()->debug("[PNT]Loading image from path \"{}\"", path);

        if(m_pixels != nullptr) {
            stbi_image_free(m_pixels);
        }
        m_pixels = stbi_load(path.c_str(), (int*)&m_width, (int*)&m_height, (int*)&this->m_channels, 4);
        if(m_pixels == nullptr) {
            logger.get()->warn("Failed to load image");
        }
    }

    void image::ImGuiDraw(ImVec2 dimensions) const {
        if(m_textureID) {
            ImGui::Image((void*)(intptr_t)m_textureID, dimensions);
        } else {
            ImGui::Text("Image not loaded on GPU");
        }
    }

    void image::ImGuiDraw(uint32_t width, uint32_t height) const {
        if(m_textureID) {
            ImGui::Image((void*)(intptr_t)m_textureID, ImVec2((float)width, (float)height));
        } else {
            ImGui::Text("Image not loaded on GPU");
        }
    }

    void image::imageSettings(unsigned int min, unsigned int mag, unsigned int S, unsigned int T) {
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T);
    }

    void image::loadOnGPU() {
        logger.get()->debug("[PNT]Loading image onto GPU with width: {} and height: {}", m_width, m_height);

        if(!m_textureID) {
            glGenTextures(1, &m_textureID);
            glBindTexture(GL_TEXTURE_2D, m_textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    void image::unloadOffGPU() {
        if(m_textureID) {
            logger.get()->debug("[PNT]Unloading image off GPU");

            glDeleteTextures(1, &m_textureID);
            m_textureID = 0;
        } else {
            logger.get()->warn("[PNT]Tried to unload a off GPU when image was not on GPU");
        }
    }

    uint32_t image::getID() const {
        return m_textureID;
    }

    uint32_t image::getWidth() const {
        return m_width;
    }

    uint32_t image::getHeight() const {
        return m_height;
    }

    unsigned char* image::getPixels() const {
        return m_pixels;
    }

    bool image::valid() const {
        return m_pixels == nullptr ? false : true;
    }
}
