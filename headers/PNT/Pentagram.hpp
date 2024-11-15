#pragma once

#include <PNT/init.hpp>
#include <PNT/window.hpp>
#include <PNT/event.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <glad/gl.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/ext.hpp>
#include <stb_image.h>
#include <glm/glm.hpp>

inline auto userConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
inline auto userFileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/", 0, 0, true);
inline std::vector<spdlog::sink_ptr> userSinks{userConsoleSink, userFileSink};
#ifndef PNT_USER_LOGGER_NAME
inline std::shared_ptr<spdlog::logger> userLogger = std::make_shared<spdlog::logger>("Log", userSinks.begin(), userSinks.end());
#endif
#ifdef PNT_USER_LOGGER_NAME
inline std::shared_ptr<spdlog::logger> userLogger = std::make_shared<spdlog::logger>(PNT_USER_LOGGER_NAME, userSinks.begin(), userSinks.end());
#endif
