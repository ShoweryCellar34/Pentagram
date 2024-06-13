cmake_minimum_required(VERSION 3.13)
project(Vendors LANGUAGES CXX C)
add_library(Vendors STATIC)
set_target_properties(Vendors PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Vendors PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad)
target_sources(Vendors PUBLIC glad/glad.c)

set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
add_subdirectory(glfw)
target_include_directories(Vendors PUBLIC ${GLFW_INCLUDE_DIRS})
target_sources(Vendors PUBLIC ${GLFW_LIBRARIES})

target_include_directories(Vendors PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/ ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp)
target_sources(Vendors PUBLIC imgui/imgui.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui/misc/cpp/imgui_stdlib.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)

target_include_directories(Vendors PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb)

add_subdirectory(glm)
target_link_libraries(Vendors )

add_subdirectory(spdlog)
target_link_libraries(Vendors PUBLIC spdlog::spdlog glm::glm)

find_package(OpenGL REQUIRED)
