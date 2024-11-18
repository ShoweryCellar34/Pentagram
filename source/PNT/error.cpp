#include <PNT/error.hpp>

#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

namespace PNT {
#ifndef PNT_NO_CONSOLE_LOG
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("pentagramLogs/", 0, 0, true);
    std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};
    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("Pentagram", sinks.begin(), sinks.end());
#endif
#ifdef PNT_NO_CONSOLE_LOG
    auto fileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("pentagramLogs/", 0, 0, true);
    std::vector<spdlog::sink_ptr> sinks{fileSink};
    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("Pentagram", sinks.begin(), sinks.end());
#endif

    exception::exception(const std::string& message, errorCodes errorCode) : m_message(message), m_errorCode(errorCode) {
    }

    const char* exception::what() const throw() {
        return m_message.c_str();
    }

    errorCodes exception::whatErrorCode() const {
        return m_errorCode;
    }

    void errorCallback(int errorCode, const char* errorDescription) {
        throw exception((std::string)errorDescription, errorCodes::GLFW_ERROR);
    }
}
