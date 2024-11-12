cmake_minimum_required(VERSION 3.8)
project(glad)
set(CMAKE_C_STANDARD 11)

add_library(glad STATIC src/glad.c include/glad/glad.h include/KHR/khrplatform.h)
target_include_directories(glad PUBLIC include/)
