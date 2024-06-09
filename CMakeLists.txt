cmake_minimum_required(VERSION 3.10)
project(Pentagram DESCRIPTION "A framwork for building apps" LANGUAGES CXX HOMEPAGE_URL "https://github.com/ShoweryCellar34/Pentagram")
add_library(Pentagram STATIC)
set_target_properties(Pentagram PROPERTIES LINKER_LANGUAGE CXX)

set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
add_subdirectory(glfw)
target_include_directories(Pentagram PUBLIC glfw)
target_link_libraries(Pentagram PUBLIC glfw)

set(glfw_opengl3 ON CACHE BOOL "glfw_opengl3")
get_target_property(GLFW_header_path glfw INTERFACE_INCLUDE_DIRECTORIES)
set(include_paths ${GLFW_header_path} CACHE STRING "include_paths")
add_subdirectory(imgui)
target_include_directories(Pentagram PUBLIC imgui)
target_link_libraries(Pentagram PUBLIC imgui)

add_subdirectory(glm)
target_include_directories(Pentagram PUBLIC glm)

add_subdirectory(glad)
target_include_directories(Pentagram PUBLIC glad)
target_link_libraries(Pentagram PUBLIC glad)

add_subdirectory(spdlog)
target_link_libraries(Pentagram PUBLIC spdlog)

target_include_directories(Pentagram PUBLIC "stb/")

add_subdirectory(PentagramHeaders)
target_include_directories(Pentagram PUBLIC PentagramHeaders)
target_link_libraries(Pentagram PUBLIC PentagramHeaders)
