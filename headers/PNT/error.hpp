#pragma once

#include <exception>
#include <string>

namespace PNT {
    enum class errorCodes {
        PNT_ERROR,
        GLFW_ERROR
    };

    class exception : std::exception {
    private:
        std::string m_message;
        errorCodes m_errorCode;

    public:
        exception(const std::string& message, errorCodes errorCode);

        const char* what() const throw();
        errorCodes whatErrorCode() const;
    };

    void errorCallback(int errorCode, const char* errorDescription);
}
