#pragma once

#include "includes.hpp"

namespace PNT {
    /// @brief File handling class for getting and setting contents of a file.
    class file {
    private:
        char* path = new char[1];
        size_t readPosition = 0;
        size_t writePosition = 0;
        std::fstream fileStream;

    public:
        file() {}
        /// @brief File object constuctor.
        /// @param path The desired file path to do operations on (will not be opened till read or write call, then closed immediately after).
        file(const char* path) {
            this->path = new char[strlen(path)];
            strcpy(this->path, path);
        }
        ~file() {
            delete[] path;
        }

        /// @brief Append contents to the file (Will be opened and immediatly closed afterwards).
        /// @param contents The desired contents to fill the file with.
        /// @return False if the write was successful, and true if otherwise.
        bool setContents(const char* contents) {
            fileStream.open(path);
            if(fileStream.is_open()) {
                setReadPosition(readPosition);
                setWritePosition(writePosition);
                fileStream << contents;
                fileStream.close();
                if(fileStream.good()) {
                    return false;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        }
        /// @brief Sets the position for the read and write pointers pointer, this changes the point at which read and write operations will get executed. The positions will save after closing the file, but it may cause problems if there are less characters than the position.
        /// @param position The desired position for the read and write pointers.
        /// @return False if the set was successful, and true if otherwise (this can be coused by an invalid position).
        /// @note You can use this function to skip to the end of the file for reading and appending from that position.
        bool setPosition(size_t position) {
            fileStream.open(path);
            if(fileStream.is_open()) {
                readPosition = position;
                writePosition = position;
                fileStream.seekg(position);
                fileStream.seekp(position);
                if(fileStream.good()) {
                    return false;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        }
        /// @brief Sets the position for the read pointer, this changes the point at which read operations will get executed. The positions will save after closing the file, but it may cause problems if there are less characters than the position.
        /// @param position The desired position for the read pointer.
        /// @return False if the set was successful, and true if otherwise (this can be coused by an invalid position).
        /// @note You can use this function to skip to the end of the file for reading from that position.
        bool setReadPosition(size_t position) {
            fileStream.open(path);
            if(fileStream.is_open()) {
                readPosition = position;
                fileStream.seekg(position);
                if(fileStream.good()) {
                    return false;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        }
        /// @brief Sets the position for the write pointer, this changes the point at which write operations will get executed. The positions will save after closing the file, but it may cause problems if there are less characters than the position.
        /// @param position The desired position for the write pointer.
        /// @return False if the set was successful, and true if otherwise (this can be coused by an invalid position).
        /// @note You can use this function to skip to the end of the file for appending from that position.
        bool setWritePosition(size_t position) {
            fileStream.open(path);
            if(fileStream.is_open()) {
            fileStream.open(path);
                writePosition = position;
                fileStream.seekp(position);
                if(fileStream.good()) {
                    return false;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        }

        /// @brief Gets the read pointer position.
        /// @param result A pointer to the desired varable to store the read pointer position.
        /// @return False if the get was successful, and true if otherwise.
        bool getReadPosition(size_t* result) {
            fileStream.open(path);
            if(fileStream.is_open()) {
                fileStream.open(path);
                *result = fileStream.tellg();
                if(fileStream.good()) {
                    return false;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        }
        /// @brief Gets the write pointer position.
        /// @param result A pointer to the desired varable to store the write pointer position.
        /// @return False if the get was successful, and true if otherwise.
        bool getReadPosition(size_t* result) {
            fileStream.open(path);
            if(fileStream.is_open()) {
                fileStream.open(path);
                *result = fileStream.tellg();
                if(fileStream.good()) {
                    return false;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        }
        size_t getCharacterCount() {
            if(fileStream.is_open()) {
                size_t oldPosition = getReadPosition();
                fileStream.seekg(0);
                size_t firstPosition = fileStream.tellg(); 
                
                if(fileStream.good()) {
                    return false;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        }
    };
}
