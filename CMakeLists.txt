cmake_minimum_required(VERSION 3.22)
project(OpenGL_Tutorial CXX)

set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
elseif (UNIX)
endif ()

add_executable(sample main.cpp Object.h Shape.h window.h)

target_compile_options(sample PRIVATE -g -Wall --pedantic-errors)

if (APPLE)
    target_link_libraries(
            sample glfw GLEW
            "-framework OpenGL" "-framework CoreVideo"
            "-framework IOKit" "-framework Cocoa"
    )
elseif (UNIX)
    target_link_libraries(
            sample glfw GLEW GL
            Xrandr Xcursor Xi
            Xxf86vm X11 pthread rt m dl
    )
endif ()

file(COPY_FILE ./point.vert ./build/point.vert)
file(COPY_FILE ./point.frag ./build/point.frag)
