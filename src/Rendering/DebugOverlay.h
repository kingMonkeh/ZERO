#pragma once

class Framebuffer;

class DebugOverlay {
public:
    void update(float deltaTime);
    void render(Framebuffer& framebuffer) const;

private:
    float fpsTimer = 0.0f;
    int frameCount = 0;
    int currentFPS = 0;
};
