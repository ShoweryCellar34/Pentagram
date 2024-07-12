cmake_minimum_required(VERSION 3.10)
project(Pentagram LANGUAGES CXX C DESCRIPTION "A framwork for building apps" HOMEPAGE_URL "https://github.com/ShoweryCellar34/Pentagram")

set(GLFW_LIBRARY_TYPE "OBJECT" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
add_subdirectory(vendors/glfw)

add_subdirectory(vendors/glm)

add_subdirectory(vendors/spdlog)

set(STB_LIBRARY_TYPE "OBJECT")
add_subdirectory(vendors/stb)

set(IMGUI_LIBRARY_TYPE "OBJECT")
set(RELATIVE_PATH "vendors/glfw/include")
set(IMGUI_GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/${RELATIVE_PATH}")
add_subdirectory(vendors/imgui)

set(GLAD_LIBRARY_TYPE "OBJECT")
add_subdirectory(vendors/glad)

add_library(Pentagram STATIC source/event.cpp source/file.cpp source/gl.cpp source/image.cpp source/init.cpp source/window.cpp)
set_target_properties(Pentagram PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(Pentagram spdlog::spdlog glm::glm glfw imgui glad stb)
target_include_directories(Pentagram PUBLIC headers)
