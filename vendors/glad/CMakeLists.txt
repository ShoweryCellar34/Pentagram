cmake_minimum_required(VERSION 3.10)
file(GLOB sources glad/src/*.c)
add_library(glad ${GLAD_LIBRARY_TYPE} ${sources})
target_include_directories(glad PUBLIC glad/include)
