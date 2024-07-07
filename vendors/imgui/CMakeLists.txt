cmake_minimum_required(VERSION 3.10)
add_library(imgui STATIC imgui/imgui.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui/misc/cpp/imgui_stdlib.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${IMGUI_GLFW_INCLUDE_DIRS} imgui)
