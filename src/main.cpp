
#include <SDL.h>
#include <iostream>
#include <vector>
#include "Geometry/Cube.h"
#include "Geometry/Mesh.h"
#include "Geometry/SceneObject.h"
#include "Math/Transform.h"
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

    Color red = {255, 0, 0};
    Color blue = {0, 255, 0};
    Color green = {0, 0, 255};
    Color colors [3] = {red, blue, green};

    Color white = {255, 255, 255};
    framebuffer.clear(white);

    std::vector<SceneObject> objects;
    objects.push_back({createCube(), {0, 0, 5}, 0.0f});
    objects.push_back({createCube(), {2, 0, 5}, 0.0f});
    objects.push_back({createCube(), {-2, 0, 5}, 0.0f});

    int colorPicker = 0;
    int culled = 0;
    // Draw all objects
    for (const SceneObject& object : objects) {

        const Mesh& mesh = object.mesh;
        std::vector<Vec3> transformedVertices;

        for (const Vec3& vertex : mesh.vertices) {
            Vec3 rotated = rotateY(vertex, object.rotationY);
            Vec3 transformed = translate(rotated, object.position);
            transformedVertices.push_back(transformed);
        }

        for (const Triangle& triangle : mesh.triangles) {
            Vec3 a = transformedVertices[triangle.a];
            Vec3 b = transformedVertices[triangle.b];
            Vec3 c = transformedVertices[triangle.c];

            if (isBackFacing(a, b, c, cameraPosition)) {
                culled++;
                continue;
            }

            Vec2 p1_projected = camera.project(a, window_width, window_height);
            Vec2 p2_projected = camera.project(b, window_width, window_height);
            Vec2 p3_projected = camera.project(c, window_width, window_height);

            framebuffer.drawTriangle(p1_projected, p2_projected, p3_projected, colors[colorPicker++]);
            colorPicker %= 3;

        }

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
