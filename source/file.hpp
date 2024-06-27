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
        /// @brief Sets the position for the read and write pointers pointer, this changes the point at which read and write operations will get executed
        /// @param position The desired position for the read and write pointers.
        /// @note You can use this function to skip to the end of the file for reading and appending from that position.
        void setPosition(size_t position) {
            if(fileStream.is_open()) {
                fileStream.seekg(position);
                fileStream.seekp(position);
                if(!fileStream.good()) {
                    setError("Integrity check failed!\0");
                }
            } else {
                setError("File not open!\0");
            }
        }
        /// @brief Sets the position for the read pointer, this changes the point at which read operations will get executed.
        /// @param position The desired position for the read pointer.
        /// @note You can use this function to skip to the end of the file for reading from that position.
        void setReadPosition(size_t position) {
            if(fileStream.is_open()) {
                fileStream.seekg(position);
                if(!fileStream.good()) {
                    setError("Integrity check failed!\0");
                }
            } else {
                setError("File not open!\0");
            }
        }
        /// @brief Sets the position for the write pointer, this changes the point at which write operations will get executed.
        /// @param position The desired position for the write pointer.
        /// @note You can use this function to skip to the end of the file for appending from that position.
        void setWritePosition(size_t position) {
            if(fileStream.is_open()) {
                fileStream.seekp(position);
                if(!fileStream.good()) {
                    setError("Integrity check failed!\0");
                }
            } else {
                setError("File not open!\0");
            }
        }

        /// @brief Gets the read pointer position.
        /// @return The read pointer position.
        size_t getReadPosition() {
            if(fileStream.is_open()) {
                size_t result = fileStream.tellg();
                if(fileStream.good()) {
                    return result;
                } else {
                    setError("Integrity check failed!\0");
                }
            } else {
                setError("File not open!\0");
            }
        }
        /// @brief Gets the write pointer position.
        /// @return The write pointer position.
        size_t getWritePosition() {
            if(fileStream.is_open()) {
                size_t result = fileStream.tellg();
                if(fileStream.good()) {
                    return result;
                } else {
                    setError("Integrity check failed!\0");
                }
            } else {
                setError("File not open!\0");
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
