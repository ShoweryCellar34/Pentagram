#pragma once

#include "init.hpp"
#include "window.hpp"
#include "event.hpp"
#include "enumerations.hpp"

#include <glad/gl.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/ext.hpp>
#include <stb_image.h>
#include <glm/glm.hpp>

extern std::shared_ptr<spdlog::logger> userLogger;
