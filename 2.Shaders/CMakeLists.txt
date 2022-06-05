cmake_minimum_required(VERSION 3.0.0)
project(Shaders VERSION 0.1.0)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shader.cpp
    )

set(INC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
    )

# include the source for the imgui library
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/external/imgui IMGUI_SOURCE)
set(SRC ${SRC} ${IMGUI_SOURCE})

include_directories(${INC})


# libraries

# glew
# set(glew_DIR /usr/share/cmake-3.16/Modules/)
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# OpenGL
find_package(OpenGL REQUIRED)
# include_directories(${OPENGL_gl_INCLUDE_DIRS})
link_libraries(${OPENGL_gl_LIBRARY})   

# GLFW
find_package(glfw3 REQUIRED)
link_libraries(glfw)

# generate external resources
# External resources
# find_library(IMGUI_LIB NAMES imgui imgui_lib HINTS "/home/jjjurado/Dev/OpenGL/build/2.Shaders/")
# if(NOT IMGUI_LIB)
#     message("Shaders: Imgui library not found. Creating it in /home/jjjurado/Dev/OpenGL/build/2.Shaders/")
#     add_library(imgui STATIC
#         ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_demo.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_draw.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_impl_glfw.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_impl_opengl3.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_tables.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui.cpp
#         )
#     # find_library(IMGUI_LIB NAMES imgui imgui_lib libimgui libimgui.a HINTS "/home/jjjurado/Dev/OpenGL/build/2.Shaders/" REQUIRED)
#     set(IMGUI_LIB ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libimgui.a)
# else()
#     message("Shaders: Imgui library found. ${IMGUI_LIB}")
# endif()

# target_include_directories(IMGUI_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/)

add_executable(Shaders ${SRC})

target_link_libraries(Shaders PUBLIC)