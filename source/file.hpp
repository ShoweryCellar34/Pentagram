#pragma once

#include "includes.hpp"

namespace PNT {
    /// @brief File handling class for getting and setting contents of a file.
    class file {
    private:
        char* path = nullptr;
        char* contents = nullptr;
        FILE* cFile = nullptr;

    public:
        file() {}
        file(const char* path) {
            open(path);
            load();
        }
        ~file() {
            delete[] path;
            delete[] contents;
        }

        /// @brief Opens a file.
        /// @param path The desired file path for loading, if the file does not exist one will be created.
        void open(const char* path) {
            this->path = new char[strlen(path)];
            strcpy(this->path, path);
            cFile = fopen(path, "a");
            cFile = fopen(path, "r");
        }
        /// @brief Closes the currently open file.
        void close() {
            this->path = new char[1];
            strcpy(this->path, "\0");
            fclose(cFile);
        }
        /// @brief Loads the contents of the open file.
        void load() {
            fseek(cFile, 0, SEEK_END);
            size_t size = ftell(cFile);
            contents = new char[size];
            rewind(cFile);
            fread(contents, sizeof(char), size, cFile);
        }

        /// @brief Gets the contents of the loaded file.
        /// @return The file contents as a c string (DO NOT MODIFY).
        const char* getContents() {
            return contents;
        }

        /// @brief Sets the contents of the open file.
        /// @param contents The desired contents to replace the current.
        void setContents(const char* contents) {
            cFile = fopen(path, "w");
            this->contents = new char[strlen(contents)];
            strcpy(this->contents, contents);
            fprintf(cFile, contents);
            cFile = fopen(path, "r");
        }
        /// @brief Appends a string to the end of the open file.
        /// @param contents The desired contents to append to the end of the file.
        void append(const char* contents) {
            cFile = fopen(path, "a");
            this->contents = new char[strlen(contents)];
            strcpy(this->contents, contents);
            fprintf(cFile, contents);
            cFile = fopen(path, "r");
        }
    };
}
