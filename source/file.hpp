#pragma once

#include "includes.hpp"

namespace PNT {
    /// @brief File handling class for getting and setting contents of a file.
    class file {
    private:
        std::fstream fileStream;
        char* errorBuffer = new char[1];

        void setError(const char* errorBuffer) {
            this->errorBuffer = new char[strlen(errorBuffer)];
            strcpy(this->errorBuffer, errorBuffer);
        }

    public:
        file() {}
        /// @brief File object constuctor.
        /// @param path The desired file path to do operations on.
        file(const char* path) {
            open(path);
        }
        ~file() {
            delete[] errorBuffer;
        }

        /// @brief Append contents to the file.
        /// @param contents The desired contents to fill the file with.
        void setContents(const char* contents) {
            if(fileStream.is_open()) {
                fileStream << contents;
                if(!fileStream.good()) {
                    setError("Integrity check failed!\0");
                }
            } else {
                setError("File not open!\0");
            }
        }
        /// @brief Moves the read and write pointers to the end of the file, this is required for appending.
        void jumpToEnd() {
            if(fileStream.is_open()) {
                fileStream.seekg(0, std::ios::end);
                fileStream.seekp(0, std::ios::end);
                if(!fileStream.good()) {
                    setError("Integrity check failed!\0");
                }
            } else {
                setError("File failed to open!\0");
            }
        }
        /// @brief Moves the read and write pointers to the begining of the file.
        void jumpToStart() {
            if(fileStream.is_open()) {
                fileStream.seekg(0);
                fileStream.seekp(0);
                if(!fileStream.good()) {
                    setError("Integrity check failed!\0");
                }
            } else {
                setError("File failed to open!\0");
            }
        }

        /// @brief Gets the size of the file.
        /// @return The size of the file in characters.
        size_t getCharacterCount() {
            if(fileStream.is_open()) {
                size_t old = fileStream.tellg();
                fileStream.seekg(0);
                size_t begin = fileStream.tellg();
                fileStream.seekg(0, std::ios::end);
                size_t result = (size_t)fileStream.tellg() - begin;
                if(fileStream.good()) {
                    return result;
                } else {
                    setError("Integrity check failed!\0");
                    return 0;
                }
            } else {
                setError("File not open!\0");
                return 0;
            }
        }
        const char* getContents() {

        }
        /// @brief Gets the file operation error.
        /// @return The error buffer (DO NOT MODIFY).
        const char* getError() {
            return errorBuffer;
        }

        /// @brief Open a file, this is reqired by all operations.
        /// @param path The desired file path to open.
        /// @param seekToEnd Set this to true to set the read and write pointers to the end of the file, useful for appending.
        void open(const char* path, bool seekToEnd = false) {
            fileStream.open(path);
            if(fileStream.is_open()) {
                if(seekToEnd) {
                    fileStream.seekg(0);
                    fileStream.seekp(0);
                } else {
                    fileStream.seekg(0, std::ios::end);
                    fileStream.seekp(0, std::ios::end);
                }
                if(!fileStream.good()) {
                    setError("Integrity check failed!\0");
                }
            } else {
                setError("File failed to open!\0");
            }
        }
        /// @brief Closes the currently open file, this allows other apps to modify the file.
        void close() {
            fileStream.close();
            if(!fileStream.good()) {
                setError("Integrity check failed!\0");
            }
        }
    };
}
