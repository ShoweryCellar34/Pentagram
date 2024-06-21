#pragma once

#include "includes.hpp"

namespace PNT {
    class shader {
    private:
        unsigned int ID = 0;
        unsigned int type = 0;
        char* source = nullptr;

    public:
        /// @brief Gets the shader ID.
        /// @return Shader ID.
        unsigned int getID() {
            return ID;
        }
        /// @brief Gets the shader type.
        /// @return Shader type.
        unsigned int getType() {
            return type;
        }
        /// @brief Gets the shader source.
        /// @return The shader source code (DO NOT MODIFY).
        char* getSource() {
            return source;
        }

        void setData(char* source, unsigned int type) {

        }

        shader(char* source, unsigned int type) {
            ID = glCreateShader(type);
            glShaderSource(ID, 1, &source, NULL);
            glCompileShader(ID);
        }
        shader() {
            ID = glCreateShader(type);
            glShaderSource(ID, 1, &source, NULL);
        }
        ~shader() {glDeleteShader(ID);}
    };
}
