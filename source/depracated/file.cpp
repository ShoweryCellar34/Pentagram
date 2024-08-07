#include <depracated/file.hpp>

#include <cstring>
#include <sstream>

namespace PNT {
    // File definitions.

    file::file() {
        errorBuffer[0] = 0;
    }

    file::file(const char* path) {
        errorBuffer[0] = 0;
        open(path);
    }

    void file::setError(const char* error) {
        strcpy(errorBuffer, error);
    }

    void file::setContents(const char* contents) {
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

    void file::jumpToEnd() {
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

    void file::jumpToStart() {
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

    size_t file::getCharacterCount() {
        if(fileStream.is_open()) {
            size_t old = (size_t)fileStream.tellg();
            fileStream.seekg(0);
            size_t begin = (size_t)fileStream.tellg();
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
        }
        return 0;
    }

    std::string file::getContents() {
        if(fileStream.is_open()) {
            std::string result;
            std::filebuf* a = fileStream.rdbuf();
            std::stringstream b;
            b << a;
            result = b.str();
            switch(fileStream.rdstate()) {
            case std::ios_base::badbit:
                setError("Bad bit is set!");
                return "";
                break;
            case std::ios_base::failbit:
                setError("Fail bit is set!");
                return "";
                break;
            case std::ios_base::eofbit:
                setError("EOF bit is set!");
                return "";
                break;
            case std::ios_base::goodbit:
                return result;
                break;
            }
        } else {
            setError("No file open!");
            return "";
        }
        return "";
    }

    const char* file::getError() {
        return errorBuffer;
    }

    void file::open(const char* path) {
        fileStream.open(path);
        if(fileStream.is_open()) {
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

    void file::close() {
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
}
