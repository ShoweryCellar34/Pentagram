set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

cmake_minimum_required(VERSION 3.10)
project(Pentagram LANGUAGES CXX C DESCRIPTION "A framwork for building apps" HOMEPAGE_URL "https://github.com/ShoweryCellar34/Pentagram")
file(GLOB pentagram source/*/*.cpp)
add_library(Pentagram STATIC ${pentagram})
target_include_directories(Pentagram PUBLIC headers)
set_target_properties(Pentagram PROPERTIES LINKER_LANGUAGE CXX)

set(GLFW_USE_HYBRID_HPG true CACHE BOOL "Force use of high-performance GPU on hybrid systems")
set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
add_subdirectory(vendors/glfw)

set(GLM_ENABLE_CXX_20 true CACHE BOOL "Enable C++ 20")
add_subdirectory(vendors/glm)

set(SPDLOG_USE_STD_FORMAT true CACHE BOOL "Use std::format instead of fmt library.")
add_subdirectory(vendors/spdlog)

set(STB_LIBRARY_TYPE "STATIC")
add_subdirectory(vendors/stb)

set(IMGUI_LIBRARY_TYPE "STATIC")
set(RELATIVE_PATH "vendors/glfw/include")
set(IMGUI_GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/${RELATIVE_PATH}")
add_subdirectory(vendors/imgui)

set(GLAD_LIBRARY_TYPE "STATIC")
add_subdirectory(vendors/glad)

target_link_libraries(Pentagram spdlog::spdlog glm::glm glfw imgui glad stb)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /MP")
endif()
if(GCC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wpedantic -fsanitize=address,undefined")
endif()
