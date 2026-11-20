
#include <SDL.h>
#include <iostream>
#include <vector>
#include "Loading/STLLoader.h"
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

    Vec3 cameraPosition {3, 3, 0};
    Vec3 cameraRotation {0, 0, 0};
    Camera camera (90.0f, cameraPosition, cameraRotation);

    Color red = {255, 0, 0};
    Color blue = {0, 255, 0};
    Color green = {0, 0, 255};
    Color colors [3] = {red, blue, green};

    Color white = {255, 255, 255};

    std::vector<SceneObject> objects;
    // objects.push_back({loadBinarySTL(R"(C:\Users\gamer\Documents\GitHub\ZERO\porolick.stl)"), {0, 0, 300}, {toRadians(270.0f), toRadians(0.0f), toRadians(0.0f)}, {0.1f, 0.1f, 0.1f}});
    objects.push_back({createCube(), {0, 0, 5}, {toRadians(270.0f), toRadians(0.0f), toRadians(0.0f)}, {1.0f, 1.0f, 1.0f}});
    objects.push_back({createCube(), {2, 0, 5}, {toRadians(270.0f), toRadians(0.0f), toRadians(0.0f)}, {1.0f, 1.0f, 1.0f}});
    objects.push_back({createCube(), {-2, 0, 5}, {toRadians(270.0f), toRadians(0.0f), toRadians(0.0f)}, {1.0f, 1.0f, 1.0f}});

    std::cout << "Framebuffer created: "
              << framebuffer.getWidth() << "x"
              << framebuffer.getHeight() << '\n';

    bool running = true;
    SDL_Event event;
    Uint64 previousCounter = SDL_GetPerformanceCounter();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // Compute delta time

        Uint64 currentCounter = SDL_GetPerformanceCounter();
        float deltaTime = static_cast<float>(currentCounter - previousCounter) / SDL_GetPerformanceFrequency();
        previousCounter = currentCounter;

        // Read keyboard input

        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
        Vec3 movement{0, 0, 0};
        if (keyboardState[SDL_SCANCODE_W]) movement.z += 1;
        if (keyboardState[SDL_SCANCODE_S]) movement.z -= 1;
        if (keyboardState[SDL_SCANCODE_A]) movement.x -= 1;
        if (keyboardState[SDL_SCANCODE_D]) movement.x += 1;
        if (keyboardState[SDL_SCANCODE_SPACE]) movement.y += 1;
        if (keyboardState[SDL_SCANCODE_LSHIFT]) movement.y -= 1;

        if (movement.length() > 0.0f) {
            movement = movement.normalized();
        }

        camera.move(movement * 5.0f * deltaTime);

        Vec3 rotation{0, 0, 0};
        if (keyboardState[SDL_SCANCODE_I]) rotation.x -= 1;
        if (keyboardState[SDL_SCANCODE_K]) rotation.x += 1;
        if (keyboardState[SDL_SCANCODE_J]) rotation.y -= 1;
        if (keyboardState[SDL_SCANCODE_L]) rotation.y += 1;

        camera.rotateBy(rotation * 1.5f * deltaTime);

        // Render scene

        framebuffer.clear(white);

        int colorPicker = 0;
        int culled = 0;

        for (const SceneObject& object : objects) {

            const Mesh& mesh = object.mesh;
            std::vector<Vec3> transformedVertices;

            for (const Vec3& vertex : mesh.vertices) {
                Vec3 scaled = scale(vertex, object.scale);
                Vec3 rotated = rotateX(scaled, object.rotation.x);
                rotated = rotateY(rotated, object.rotation.y);
                rotated = rotateZ(rotated, object.rotation.z);
                Vec3 translated = translate(rotated, object.position);
                transformedVertices.push_back(translated);
            }

            for (const Triangle& triangle : mesh.triangles) {
                Vec3 a = camera.worldToCamera(transformedVertices[triangle.a]);
                Vec3 b = camera.worldToCamera(transformedVertices[triangle.b]);
                Vec3 c = camera.worldToCamera(transformedVertices[triangle.c]);

                if (isBackFacing(a, b, c, {0, 0, 0})) {
                    culled++;
                    continue;
                }

                Vec2 p1_projected = camera.project(a, window_width, window_height);
                Vec2 p2_projected = camera.project(b, window_width, window_height);
                Vec2 p3_projected = camera.project(c, window_width, window_height);

                framebuffer.drawTriangle(p1_projected, p2_projected, p3_projected, a.z, b.z, c.z, colors[colorPicker++]);
                colorPicker %= 3;

            }

        }

        // Display framebuffer

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

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}
