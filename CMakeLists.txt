set(POSITION_INDEPENDENT_CODE true)

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
set_target_properties(Pentagram PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 17)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 -D_CRT_SECURE_NO_WARNINGS")
endif()
target_link_libraries(Pentagram spdlog::spdlog glm::glm glfw imgui glad stb)
target_include_directories(Pentagram PUBLIC headers)

option(PNT_BUILD_TESTS "Build Pentagram tests" true)

if(${PNT_BUILD_TESTS})

add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/tests/res ${PROJECT_BINARY_DIR}/res COMMENT "Copying resources into binary directory")

file(GLOB tests tests/*.cpp)
foreach(test ${tests})

get_filename_component(test_name ${test} NAME_WE)
add_executable(${test_name} ${test})
target_link_libraries(${test_name} Pentagram)
add_dependencies(${test_name} copy_resources)

endforeach(test ${tests})
endif()
