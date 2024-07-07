cmake_minimum_required(VERSION 3.10)
add_library(stb STATIC stb.c)
target_include_directories(stb PUBLIC stb)
