#pragma once

#include "includes.hpp"

namespace PNT {
    class shader {
    private:
        unsigned int ID = 0;
        unsigned int type = 0;
        char* source;
    public:
        // Returns the shader ID.
        unsigned int getID() {return ID;}
        // Returns the type of shader.
        unsigned int getType() {return type;}
        // Returns the shader source (DO NOT MODIFY).
        char* getSource() {return source;}

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
