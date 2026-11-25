
#include "DebugOverlay.h"
#include "Framebuffer.h"

#include <array>
#include <string>

void DebugOverlay::update(float deltaTime) {
    fpsTimer += deltaTime;
    frameCount++;

    if (fpsTimer >= 1.0f) {
        currentFPS = static_cast<int>(frameCount / fpsTimer);

        fpsTimer = 0.0f;
        frameCount = 0;
    }
}

void DebugOverlay::render(Framebuffer& framebuffer) const {
    auto getGlyph = [](char c) -> std::array<int, 5> {
        switch (c) {
            case 'F': return {7, 4, 6, 4, 4};
            case 'P': return {6, 5, 6, 4, 4};
            case 'S': return {3, 4, 2, 1, 6};
            case ':': return {0, 2, 0, 2, 0};
            case '0': return {7, 5, 5, 5, 7};
            case '1': return {2, 6, 2, 2, 7};
            case '2': return {7, 1, 7, 4, 7};
            case '3': return {7, 1, 7, 1, 7};
            case '4': return {5, 5, 7, 1, 1};
            case '5': return {7, 4, 7, 1, 7};
            case '6': return {7, 4, 7, 5, 7};
            case '7': return {7, 1, 1, 1, 1};
            case '8': return {7, 5, 7, 5, 7};
            case '9': return {7, 5, 7, 1, 7};
            default:  return {0, 0, 0, 0, 0};
        }
    };

    const std::string text = "FPS: " + std::to_string(currentFPS);

    constexpr int scale = 3;
    constexpr int startX = 12;
    constexpr int startY = 12;

    const int textWidth = static_cast<int>(text.size()) * 4 * scale;
    const int textHeight = 5 * scale;

    // Background
    for (int y = 0; y < textHeight + 8; y++) {
        for (int x = 0; x < textWidth + 8; x++) {
            framebuffer.setPixel(startX + x, startY + y, {30, 30, 30});
        }
    }

    // Text
    int cursorX = startX + 4;
    int cursorY = startY + 4;

    for (char c : text) {
        const auto glyph = getGlyph(c);

        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 3; col++) {
                if (glyph[row] & (1 << (2 - col))) {
                    for (int sy = 0; sy < scale; sy++) {
                        for (int sx = 0; sx < scale; sx++) {
                            framebuffer.setPixel(
                                cursorX + col * scale + sx,
                                cursorY + row * scale + sy,
                                {255, 255, 255}
                            );
                        }
                    }
                }
            }
        }

        cursorX += 4 * scale;
    }
}
