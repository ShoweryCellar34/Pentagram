#pragma once

#include "init.hpp"
#include "window.hpp"
#include "event.hpp"
#include "enumerations.hpp"
#include "gl.hpp"
#include "image.hpp"

#include <glad/gl.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/ext.hpp>
#include <stb_image.h>

extern std::shared_ptr<spdlog::logger> userLogger;
