
#include <SDL.h>
#include <iostream>
#include "Math/Vec3.h"
#include "Math/Utils.h"
#include "Rendering/Framebuffer.h"
#include "Rendering/Camera.h"
#include "Rendering/Culling.h"

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    int window_width = 1280;
    int window_height = 720;
    SDL_Window* window = SDL_CreateWindow(
        "ZERO Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_SOFTWARE
    );

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );

    Framebuffer framebuffer (window_width, window_height);

    Vec3 cameraPosition {0, 0, 0};
    Camera camera (90.0f);

    Color white = {255, 255, 255};
    Color red = {255, 0, 0};
    framebuffer.clear(white);
    
    Vec3 vertices[] = {
        {-1, -1, -1}, // 0
        { 1, -1, -1}, // 1
        { 1,  1, -1}, // 2
        {-1,  1, -1}, // 3

        {-1, -1,  1}, // 4
        { 1, -1,  1}, // 5
        { 1,  1,  1}, // 6
        {-1,  1,  1}  // 7
    };

    // Move around the cube for testing
        float angle = toRadians(30.0f);

    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);

    for (int i = 0; i < 8; i++) {
        Vec3 original = vertices[i];

        vertices[i].x = original.x * cosAngle + original.z * sinAngle;
        vertices[i].z = -original.x * sinAngle + original.z * cosAngle;

        // Move the rotated cube in front of the camera.
        vertices[i].z += 5.0f;
    }

    int triangles[][3] = {
        // Back face
        {0, 2, 1},
        {0, 3, 2},

        // Front face
        {4, 5, 6},
        {4, 6, 7},

        // Left face
        {0, 4, 7},
        {0, 7, 3},

        // Right face
        {1, 2, 6},
        {1, 6, 5},

        // Bottom face
        {0, 1, 5},
        {0, 5, 4},

        // Top face
        {3, 7, 6},
        {3, 6, 2}
    };

    int culled = 0;
    for (int i = 0; i < 12; i++) {
        Vec3 a = vertices[triangles[i][0]];
        Vec3 b = vertices[triangles[i][1]];
        Vec3 c = vertices[triangles[i][2]];

        if (isBackFacing(a, b, c, cameraPosition)) {
            culled++;
            continue;
        }

        Vec2 p1_projected = camera.project(a, window_width, window_height);
        Vec2 p2_projected = camera.project(b, window_width, window_height);
        Vec2 p3_projected = camera.project(c, window_width, window_height);

        framebuffer.drawTriangle(p1_projected, p2_projected, p3_projected, red);
    }

    std::cout << "Culled triangles: " << culled << '\n';

    std::cout << "Framebuffer created: "
              << framebuffer.getWidth() << "x"
              << framebuffer.getHeight() << '\n';

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        SDL_UpdateTexture(
          texture,
          nullptr,
          framebuffer.getPixels(),
          window_width * sizeof(Color)
        );

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}
