#include <image.hpp>

#include <stb_image.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad.h>

namespace PNT {
    // Image definitions.

    image::image() : width(100), height(100) {
    };

    image::image(const char* path) {
        load(path);
    }

    image::image(const image& original) {
        width = original.width;
        height = original.height;
        channels = original.channels;
        pixels = original.pixels;
        strcpy((char*)pixels, (const char*)original.pixels);
        if(original.textureID) {
            loadOnGPU();
        }
    }

    image::~image() {
        if(textureID) {
            unloadOffGPU();
        }
        stbi_image_free(pixels);
    }

    void image::load(const char* path) {
        pixels = stbi_load(path, &width, &height, &this->channels, 4);
    }

    void image::ImGuiDraw(ImVec2 dimensions) const {
        if(textureID) {
            ImGui::Image((void*)(intptr_t)textureID, dimensions);
        } else {
            ImGui::Text("Image not loaded on GPU");
        }
    }

    void image::ImGuiDraw(uint32_t width, uint32_t height) const {
        if(textureID) {
            ImGui::Image((void*)(intptr_t)textureID, ImVec2(width, height));
        } else {
            ImGui::Text("Image not loaded on GPU");
        }
    }

    void image::imageSettings(unsigned int min, unsigned int mag, unsigned int S, unsigned int T) {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T);
    }

    void image::loadOnGPU() {
        if(!textureID) {
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    void image::unloadOffGPU() {
        if(textureID) {
            glDeleteTextures(1, &textureID);
            textureID = 0;
        }
    }

    uint32_t image::getID() const {
        return textureID;
    }

    uint32_t image::getWidth() const {
        return width;
    }

    uint32_t image::getHeight() const {
        return height;
    }

    unsigned char* image::getPixels() const {
        return pixels;
    }

    bool image::valid() const {
        return pixels == nullptr ? false : true;
    }
}
