
cmake_minimum_required(VERSION 3.20)

project(ZERO
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------
# Dependencies
# --------------------------------

include(FetchContent)

# Build SDL2 as a static library
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)

# Download SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.10
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL2)

# --------------------------------
# ZERO Engine
# --------------------------------

add_executable(ZERO
    src/main.cpp
    src/Rendering/Framebuffer.cpp
    src/Math/Vec3.cpp
)

# Link SDL2
if(TARGET SDL2::SDL2main)
    target_link_libraries(ZERO PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(ZERO PRIVATE SDL2::SDL2-static)
