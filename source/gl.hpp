#pragma once

#include "includes.hpp"

namespace PNT {
    class shader {
    private:
        unsigned int shaderID = 0;
        unsigned int type = 0;
        char* source = nullptr;

    public:
        /// @brief Gets the shader ID.
        /// @return Shader ID.
        unsigned int getID() {
            return shaderID;
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

        /// @brief Compiles the shader.
        void compile() {
            glCompileShader(shaderID);
        }

        void setData(const char* source) {
            glShaderSource(shaderID, 1, &source, NULL);
        }

        shader(const char* source, unsigned int type) {
            shaderID = glCreateShader(type);
            glShaderSource(shaderID, 1, &source, NULL);
            glCompileShader(shaderID);
        }
        shader() {}
        ~shader() {
            glDeleteShader(shaderID);
        }
    };
}
