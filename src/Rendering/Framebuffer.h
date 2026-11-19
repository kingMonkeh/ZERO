#pragma once

#include "../Math/Vec2.h"
#include <vector>

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

class Framebuffer {
  public:
    Framebuffer(int width, int height);
    void setPixel(int x, int y, Color color);
    void clear(Color color);
    int getWidth() const;
    int getHeight() const;
    const Color* getPixels() const;
    void drawTriangle(
        const Vec2& a,
        const Vec2& b,
        const Vec2& c,
        Color color
    );

  private:
    int width;
    int height;
    std::vector<Color> pixels;

};
