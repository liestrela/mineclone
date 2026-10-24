cmake_minimum_required(VERSION 3.11)
project(mineclone VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(CMAKE_CXX_STANDARD 23)

# glm stuff
FetchContent_Declare(
    glm
    GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

# glew stuff
FetchContent_Declare(
    glew
    GIT_REPOSITORY	https://github.com/Perlmint/glew-cmake.git
	GIT_TAG 	origin/master
)
FetchContent_MakeAvailable(glew)

# sdl2 stuff
set(SDL2_IMAGE_INSTALL OFF)
set(SDL2_DISABLE_INSTALL OFF)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY	https://github.com/libsdl-org/SDL.git
	GIT_TAG 	release-2.32.10
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL2)

# sd2_image stuff
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY	https://github.com/libsdl-org/SDL_image.git
	GIT_TAG 	release-2.6.x
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL2_image)

add_executable(mineclone src/main.cpp)
target_link_libraries(mineclone PRIVATE glm::glm SDL2::SDL2 SDL2_image::SDL2_image libglew_static)