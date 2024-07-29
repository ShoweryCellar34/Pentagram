#pragma once

#include <fstream>
#include <stdint.h>

namespace PNT {
    /// @brief File handling class for getting and setting contents of a file.
    class file {
    private:
        std::fstream fileStream;
        char errorBuffer[256];

        void setError(const char* error);

    public:
        file();

        /// @brief File object constuctor.
        /// @param path The desired file path to do operations on.
        file(const char* path);

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
        std::string getContents();

        /// @brief Gets the file operation error.
        /// @return The error buffer).
        const char* getError();

        /// @brief Open a file, this is reqired by all operations.
        /// @param path The desired file path to open.
        /// @param seekToEnd Set this to true to set the read and write pointers to the end of the file, useful for appending.
        void open(const char* path);

        /// @brief Closes the currently open file, this allows other apps to modify the file (call close before open if reusing the object).
        void close();
    };
}
