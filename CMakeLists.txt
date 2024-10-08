set(POSITION_INDEPENDENT_CODE true)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

cmake_minimum_required(VERSION 3.10)
project(Pentagram LANGUAGES CXX C DESCRIPTION "A framwork for building apps" HOMEPAGE_URL "https://github.com/ShoweryCellar34/Pentagram")

set(GLFW_USE_HIBRID_HPG true)
set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
add_subdirectory(vendors/glfw)

add_subdirectory(vendors/glm)

add_subdirectory(vendors/spdlog)

set(STB_LIBRARY_TYPE "STATIC")
add_subdirectory(vendors/stb)

set(IMGUI_LIBRARY_TYPE "STATIC")
set(RELATIVE_PATH "vendors/glfw/include")
set(IMGUI_GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/${RELATIVE_PATH}")
add_subdirectory(vendors/imgui)

set(GLAD_LIBRARY_TYPE "STATIC")
add_subdirectory(vendors/glad)

file(GLOB pentagram source/*.cpp)
add_library(Pentagram STATIC ${pentagram})
set_target_properties(Pentagram PROPERTIES LINKER_LANGUAGE CXX)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /MP")
endif()
if(GCC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic -fsanitize=address,undefined")
endif()
target_link_libraries(Pentagram spdlog::spdlog glm::glm glfw imgui glad stb)
target_include_directories(Pentagram PUBLIC headers)
