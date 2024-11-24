cmake_minimum_required(VERSION 3.8)
project(DuperGame C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libs/glad EXCLUDE_FROM_ALL)
add_subdirectory(libs/glfw EXCLUDE_FROM_ALL)
add_subdirectory(libs/cglm EXCLUDE_FROM_ALL)
add_subdirectory(libs/cimgui EXCLUDE_FROM_ALL)

include_directories(./libs/glad/include ./libs/stb)

option(TREAT_WARNINGS_AS_ERRORS "Treat warnings as errors" ON) # Default to ON for safer code
if (TREAT_WARNINGS_AS_ERRORS)
    if (MSVC)
        add_compile_options(/WX)
    else()
        add_compile_options(-Werror)
    endif()
endif()


set(LIBS glad glfw cglm cimgui m)

add_executable(DuperGame
        src/main.c
        src/gfx/shader.c
        src/gfx/shader.h
        src/gfx/textures.c
        src/gfx/textures.h
        src/gfx/camera.c
        src/gfx/camera.h
        src/gfx/gui.c
        src/gfx/gui.h)

target_compile_definitions(cimgui PRIVATE CIMGUI_USE_GLFW CIMGUI_USE_OPENGL3)
target_link_libraries(DuperGame PRIVATE ${LIBS})

add_custom_command(TARGET DuperGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:DuperGame>/res)

add_custom_target(run
        COMMAND DuperGame
        DEPENDS DuperGame
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
        COMMENT "Running DuperGame")
