cmake_minimum_required(VERSION 3.10)
add_library(glad STATIC glad/glad.c)
target_include_directories(glad PUBLIC glad)
