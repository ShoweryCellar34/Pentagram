cmake_minimum_required(VERSION 3.10)
file(GLOB glad_sources glad/src/*.c)
add_library(glad ${GLAD_LIBRARY_TYPE} ${glad_sources})
target_include_directories(glad PUBLIC glad/include)
