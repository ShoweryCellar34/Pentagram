#pragma once

#include <fstream>
#include <string>
#include <cstdint>

namespace PNT {
    /// @brief File handling class for getting and setting contents of a file.
    class file {
    private:
        std::fstream fileStream;
        std::string errorBuffer;

    public:
        file();

        /// @brief File object constuctor.
        /// @param path The desired file path to do operations on.
        file(const char* path)  {
            open(path);
        }

        /// @brief Append contents to the file.
        /// @param contents The desired contents to fill the file with.
        void setContents(const char* contents);

        /// @brief Moves the read and write pointers to the end of the file, this is required for appending.
        void jumpToEnd();

        /// @brief Moves the read and write pointers to the begining of the file.
        void jumpToStart();

        /// @brief Gets the size of the file.
        /// @return The size of the file in characters.
        size_t getCharacterCount();

        /// @brief Gets the contents of the currently open file.
        /// @return The contents of the file.
        std::string getContents() {
        if(fileStream.is_open()) {
            fileStream.seekg(0, std::ios::end);
            size_t size = fileStream.tellg();

            std::string result(size, ' ');
            fileStream.seekg(0);
            fileStream.read(&result[0], size); 

            switch(fileStream.rdstate()) {
            case std::ios_base::badbit:
                errorBuffer = "Bad bit is set!";
                return "";
                break;
            case std::ios_base::failbit:
                errorBuffer = "Fail bit is set!";
                return "";
                break;
            case std::ios_base::eofbit:
                errorBuffer = "EOF bit is set!";
                return "";
                break;
            case std::ios_base::goodbit:
                return result;
                break;
            }
        } else {
            errorBuffer = "No file open!";
            return "";
        }
        return "";
    }

        /// @brief Gets the file operation error.
        /// @return The error buffer).
        std::string getError();

        /// @brief Open a file, this is reqired by all operations.
        /// @param path The desired file path to open.
        /// @param seekToEnd Set this to true to set the read and write pointers to the end of the file, useful for appending.
    void open(const char* path) {
        fileStream.open(path);
        if(fileStream.is_open()) {
            switch(fileStream.rdstate()) {
            case std::ios_base::badbit:
                errorBuffer = "Bad bit is set!";
                break;
            case std::ios_base::failbit:
                errorBuffer = "Fail bit is set!";
                break;
            case std::ios_base::eofbit:
                errorBuffer = "EOF bit is set!";
                break;
            }
        } else {
            errorBuffer = "No file open!";
        }
    }

        /// @brief Closes the currently open file, this allows other apps to modify the file.
        void close();
    };
}
