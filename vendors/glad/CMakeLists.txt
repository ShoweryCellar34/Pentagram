cmake_minimum_required(VERSION 3.10)
add_library(glad ${GLAD_LIBRARY_TYPE} glad/glad.c)
target_include_directories(glad PUBLIC glad)
