cmake_minimum_required(VERSION 3.10)
project(Pentagram LANGUAGES CXX C DESCRIPTION "A framwork for building apps" HOMEPAGE_URL "https://github.com/ShoweryCellar34/Pentagram")
add_library(Pentagram STATIC "source/init.hpp")
set_target_properties(Pentagram PROPERTIES LINKER_LANGUAGE CXX)

set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
add_subdirectory(vendors/glfw)
add_subdirectory(vendors/glm)
add_subdirectory(vendors/spdlog)

file(GLOB pentagram source/*.hpp)
file(GLOB imgui vendors/imgui/*.cpp vendors/imgui/misc/cpp/imgui_stdlib.cpp vendors/imgui/backend/imgui_impl_glfw.cpp vendors/imgui/backend/imgui_impl_opengl3.cpp)

target_link_libraries(Pentagram spdlog::spdlog glm::glm glfw)
target_sources(Pentagram PRIVATE ${pentagram} ${imgui} vendors/glad/glad.c)
target_include_directories(Pentagram PUBLIC source vendors/imgui vendors/glad vendors/stb)
