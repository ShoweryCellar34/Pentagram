cmake_minimum_required(VERSION 3.10)
project(Pentagram DESCRIPTION "A framwork for building apps" LANGUAGES CXX HOMEPAGE_URL "https://github.com/ShoweryCellar34/Pentagram")
file(GLOB pentagramHeaders pentagramHeaders/*.hpp)
add_library(Pentagram STATIC ${pentagramHeaders})
set_target_properties(Pentagram PROPERTIES LINKER_LANGUAGE CXX)

set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
add_subdirectory(glfw)
target_include_directories(Pentagram PUBLIC glfw)
target_link_libraries(Pentagram PUBLIC glfw)

file(GLOB imguiSource imgui/*.cpp imgui/misc/cpp/imgui_stdlib.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
target_include_directories(Pentagram PUBLIC imgui/ imgui/backends imgui/misc/cpp)
target_link_libraries(Pentagram PUBLIC imgui)

add_subdirectory(glm)
target_include_directories(Pentagram PUBLIC glm)

add_subdirectory(glad)
target_include_directories(Pentagram PUBLIC glad)
target_link_libraries(Pentagram PUBLIC glad)

add_subdirectory(spdlog)
target_link_libraries(Pentagram PUBLIC spdlog)

target_include_directories(Pentagram PUBLIC "stb/")

find_package(OpenGL REQUIRED)
