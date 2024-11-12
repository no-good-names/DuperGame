cmake_minimum_required(VERSION 3.8)
project(DuperGame C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libs/glad EXCLUDE_FROM_ALL)
add_subdirectory(libs/glfw EXCLUDE_FROM_ALL)
add_subdirectory(libs/cglm EXCLUDE_FROM_ALL)
add_subdirectory(libs/cimgui EXCLUDE_FROM_ALL)

include_directories(./libs/glad/include ./libs/stb/ ./libs/glfw/include ./libs/cglm/include ./libs/cimgui/)

set(LIBS glad glfw cglm cimgui m)

add_executable(DuperGame
        src/main.c
        src/gfx/shader.c
        src/gfx/shader.h
        src/gfx/textures.c
        src/gfx/textures.h
        src/gfx/buffer.c
        src/gfx/buffer.h)
target_link_libraries(DuperGame PRIVATE ${LIBS})