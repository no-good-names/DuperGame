cmake_minimum_required(VERSION 3.8)
project(DuperGame)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")

set(CMAKE_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# GLAD should always be able to be built
add_subdirectory(libs/glad EXCLUDE_FROM_ALL)
add_subdirectory(libs/glfw EXCLUDE_FROM_ALL)
add_subdirectory(libs/cglm EXCLUDE_FROM_ALL)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable Assimp tests")
set(ASSIMP_NO_EXPORT ON CACHE BOOL "Disable Assimp exporters") # If you only need import functionality
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build Assimp as a static library")
add_subdirectory(libs/assimp EXCLUDE_FROM_ALL)

include_directories(
        ./libs/glad/include
        ./libs/stb
)

# CIMGUI + GLFW + OPENGL3 ==============================================================================================
# NOTE: I pray this stays working
if(WIN32) # to mingw work as all the others
set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif(WIN32)

#run in build dir
set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

set (CMAKE_CXX_STANDARD 11)

# general settings
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/cimgui/imgui/backends)
set(BAKENDS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/libs/cimgui/imgui/backends/")
else()
set(BAKENDS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/libs/cimgui/imgui/examples/")
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/cimgui/imgui/imgui_tables.cpp)
set(TABLES_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/libs/cimgui/imgui/imgui_tables.cpp")
else()
set(TABLES_SOURCE "")
endif()

include_directories(./libs/cimgui/imgui)
add_definitions("-DIMGUI_USER_CONFIG=\"../cimconfig.h\"")
add_definitions("-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")

include_directories(./libs/cimgui)
set(IMGUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/cimgui/cimgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/cimgui/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/cimgui/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/cimgui/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/cimgui/imgui/imgui_widgets.cpp
        ${TABLES_SOURCE}
)

set(IMGUI_LIBRARIES )

add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GLAD")

# optional adding freetype
option(IMGUI_FREETYPE "add Freetype2" OFF)

if(IMGUI_FREETYPE)
FIND_PACKAGE(freetype REQUIRED PATHS ${FREETYPE_PATH})
list(APPEND IMGUI_LIBRARIES freetype)
list(APPEND IMGUI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cimgui/imgui/misc/freetype/imgui_freetype.cpp)
add_definitions("-DCIMGUI_FREETYPE=1")
endif(IMGUI_FREETYPE)

# opengl3
list(APPEND IMGUI_SOURCES ${BAKENDS_FOLDER}imgui_impl_opengl3.cpp)

if(WIN32)
list(APPEND IMGUI_LIBRARIES opengl32)
else(WIN32) # Unix
list(APPEND IMGUI_LIBRARIES GL)
endif(WIN32)

# GLFW
list(APPEND IMGUI_SOURCES ${BAKENDS_FOLDER}/imgui_impl_glfw.cpp)

add_library(cimgui STATIC ${IMGUI_SOURCES})

if (WIN32)
target_compile_definitions(cimgui PUBLIC "-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
else(WIN32)
target_compile_definitions(cimgui PUBLIC "-DIMGUI_IMPL_API=extern \"C\" ")
endif(WIN32)

target_link_libraries(cimgui ${IMGUI_LIBRARIES} glfw)

# using library
include_directories(./libs/cimgui/generator/output/)
# CIMMGUI END ==========================================================================================================

if ( MSVC )
    target_compile_definitions(assimp -D_SCL_SECURE_NO_WARNINGS )
    target_compile_definitions(assimp -D_CRT_SECURE_NO_WARNINGS )
endif ( MSVC )

option(TREAT_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF) # Default to ON for safer code
if (TREAT_WARNINGS_AS_ERRORS)
    if (MSVC)
        add_compile_options(/WX)
    else()
        add_compile_options(-Werror)
    endif()
endif()

set(LIBS glad glfw cglm cimgui assimp m)

include_directories(./libs/cimgui/generator/output)
add_executable(DuperGame
        src/main.c
        src/gfx/shader.c
        src/gfx/shader.h
        src/gfx/textures.c
        src/gfx/textures.h
        src/gfx/camera.c
        src/gfx/camera.h
        src/gfx/gui.c
        src/gfx/gui.h
        src/gfx/mesh.c
        src/gfx/mesh.h
)

target_compile_definitions(cimgui PUBLIC -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_GLFW)
target_link_libraries(DuperGame PRIVATE ${LIBS})

add_custom_command(TARGET DuperGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:DuperGame>/res)

add_custom_target(run
        COMMAND DuperGame
        DEPENDS DuperGame
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
        COMMENT "Running DuperGame")

# Return 0 if cmake exists
add_custom_target(cmake_exists
        COMMAND ${CMAKE_COMMAND} -E echo "CMake exists"
        COMMENT "Checks if CMake exists")
