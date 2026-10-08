cmake_minimum_required(VERSION 3.10.0)
project(mineclone VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(lib/sdl2 lib/glew)

set(CMAKE_CXX_STANDARD 20)

add_executable(mineclone src/main.cpp)

target_link_libraries(mineclone PRIVATE SDL2
                                PRIVATE OpenGL
                                PRIVATE GLEW)