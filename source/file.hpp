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
        /// @brief File object constuctor.
        /// @param path The desired path of the file to load.
        file(const char* path) {
            open(path);
            load();
        }
        /// @brief File object copy constuctor.
        /// @param original File object to copy from.
        file(file& original) {
            path = new char[strlen(original.path)];
            strcpy(path, (const char*)original.path);
            contents = new char[strlen(original.contents)];
            strcpy(contents, (const char*)original.contents);
            cFile = original.cFile;
        }
        ~file() {
            delete[] path;
            delete[] contents;
        }

        /// @brief Opens a file.
        /// @param path The desired file path for loading.
        void open(const char* path) {
            this->path = new char[strlen(path)];
            strcpy(this->path, path);
            cFile = fopen(path, "r");
        }
        /// @brief Closes the currently open file.
        void close() {
            this->path = new char[0];
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
