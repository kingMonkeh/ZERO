#pragma once

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
        int x0, int y0,
        int x1, int y1,
        int x2, int y2,
        Color color
    );

  private:
    int width;
    int height;
    std::vector<Color> pixels;

};
