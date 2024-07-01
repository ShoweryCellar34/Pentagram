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
                switch(fileStream.rdstate()) {
                case std::ios_base::badbit:
                    setError("Bad bit is set!");
                    break;
                case std::ios_base::failbit:
                    setError("Fail bit is set!");
                    break;
                case std::ios_base::eofbit:
                    setError("EOF bit is set!");
                    break;
                }
            } else {
                setError("No file open!");
            }
        }
        /// @brief Moves the read and write pointers to the end of the file, this is required for appending.
        void jumpToEnd() {
            if(fileStream.is_open()) {
                fileStream.seekg(0, std::ios::end);
                fileStream.seekp(0, std::ios::end);
                switch(fileStream.rdstate()) {
                case std::ios_base::badbit:
                    setError("Bad bit is set!");
                    break;
                case std::ios_base::failbit:
                    setError("Fail bit is set!");
                    break;
                case std::ios_base::eofbit:
                    setError("EOF bit is set!");
                    break;
                }
            } else {
                setError("No file open!");
            }
        }
        /// @brief Moves the read and write pointers to the begining of the file.
        void jumpToStart() {
            if(fileStream.is_open()) {
                fileStream.seekg(0, std::ios::beg);
                fileStream.seekp(0, std::ios::beg);
                switch(fileStream.rdstate()) {
                case std::ios_base::badbit:
                    setError("Bad bit is set!");
                    break;
                case std::ios_base::failbit:
                    setError("Fail bit is set!");
                    break;
                case std::ios_base::eofbit:
                    setError("EOF bit is set!");
                    break;
                }
            } else {
                setError("No file open!");
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
                switch(fileStream.rdstate()) {
                case std::ios_base::badbit:
                    setError("Bad bit is set!");
                    return 0;
                    break;
                case std::ios_base::failbit:
                    setError("Fail bit is set!");
                    return 0;
                    break;
                case std::ios_base::eofbit:
                    setError("EOF bit is set!");
                    return 0;
                    break;
                case std::ios_base::goodbit:
                    return result;
                    break;
                }
            } else {
                setError("No file open!");
                return 0;
            }
        }
        /// @brief Gets the contents of the currently open file.
        /// @return The contents of the file.
        const char* getContents() {
            if(fileStream.is_open()) {
                size_t oldPosition = fileStream.tellg();
                fileStream.seekg(0, std::ios::end);
                size_t length = (size_t)fileStream.tellg();
                fileStream.seekg(0, std::ios::beg);

                char* result = new char[length + 1];
                for(size_t i = 0; i < length; i++) {
                    result[i] = fileStream.get();
                }
                result[length] = '\0';
                fileStream.seekg(oldPosition);
                switch(fileStream.rdstate()) {
                case std::ios_base::badbit:
                    setError("Bad bit is set!");
                    return "\0";
                    break;
                case std::ios_base::failbit:
                    setError("Fail bit is set!");
                    return "\0";
                    break;
                case std::ios_base::eofbit:
                    setError("EOF bit is set!");
                    return "\0";
                    break;
                case std::ios_base::goodbit:
                    return result;
                    break;
                }
            } else {
                setError("No file open!");
                return "\0";
            }
            return "\0";
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
                    fileStream.seekg(0, std::ios::beg);
                    fileStream.seekp(0, std::ios::beg);
                }
                switch(fileStream.rdstate()) {
                case std::ios_base::badbit:
                    setError("Bad bit is set!");
                    break;
                case std::ios_base::failbit:
                    setError("Fail bit is set!");
                    break;
                case std::ios_base::eofbit:
                    setError("EOF bit is set!");
                    break;
                }
            } else {
                setError("No file open!");
            }
        }
        /// @brief Closes the currently open file, this allows other apps to modify the file.
        void close() {
            fileStream.close();
            switch(fileStream.rdstate()) {
            case std::ios_base::badbit:
                setError("Bad bit is set!");
                break;
            case std::ios_base::failbit:
                setError("Fail bit is set!");
                break;
            case std::ios_base::eofbit:
                setError("EOF bit is set!");
                break;
            }
        }
    };
}
