#include <algorithm>
#include <limits>
#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : 
  width(width), 
  height(height), 
  pixels(width * height),
  depthBuffer(width * height, std::numeric_limits<float>::infinity()) {}

void Framebuffer::setPixel(int x, int y, Color color) {
  if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
    return;
  }

  int index = y * this->width + x;
  this->pixels[index] = color;
}

void Framebuffer::clear(Color color) {
    for (size_t i = 0; i < this->pixels.size(); i++) {
        this->pixels[i] = color;
        this->depthBuffer[i] = std::numeric_limits<float>::infinity();
    }
}

int Framebuffer::getWidth() const {
    return this->width;
}

int Framebuffer::getHeight() const {
    return this->height;
}

const Color* Framebuffer::getPixels() const {
    return pixels.data();
}

static int edgeFunction(
  int ax, int ay,
  int bx, int by,
  int px, int py
) {
  // Vector AB
  int AB_x = bx - ax;
  int AB_y = by - ay;
  // Vector AP
  int AP_x = px - ax;
  int AP_y = py - ay;
  return AB_x * AP_y - AB_y * AP_x;
}

void Framebuffer::drawTriangle(
    const Vec2& a,
    const Vec2& b,
    const Vec2& c,
    float zA,
    float zB,
    float zC,
    Color color
) {

  int x0 = static_cast<int>(a.x);
  int y0 = static_cast<int>(a.y);
  int x1 = static_cast<int>(b.x);
  int y1 = static_cast<int>(b.y);
  int x2 = static_cast<int>(c.x);
  int y2 = static_cast<int>(c.y);

  int min_x = std::min({x0, x1, x2});
  int max_x = std::max({x0, x1, x2});
  int min_y = std::min({y0, y1, y2});
  int max_y = std::max({y0, y1, y2});

  min_x = std::max(min_x, 0);
  max_x = std::min(max_x, this->width - 1);
  min_y = std::max(min_y, 0);
  max_y = std::min(max_y, this->height - 1);

  // Figure out which side P3 is on relative to P1 -> P2
  int orientation = edgeFunction(x0, y0, x1, y1, x2, y2);
  for (int y = min_y; y <= max_y; y++) {
    for (int x = min_x; x <= max_x; x++) {
      int sideAB = edgeFunction(x0, y0, x1, y1, x, y);
      int sideBC = edgeFunction(x1, y1, x2, y2, x, y);
      int sideCA = edgeFunction(x2, y2, x0, y0, x, y);

      bool inside = false;
      // The point must be on the same side matching the orientation
      if (orientation > 0) {
        inside = sideAB >= 0 && sideBC >= 0 && sideCA >= 0;
      }
      else {
        inside = sideAB <= 0 && sideBC <= 0 && sideCA <= 0;
      }

      if (!inside) {
        continue;
      }

      float alpha = static_cast<float>(sideBC) / orientation;
      float beta  = static_cast<float>(sideCA) / orientation;
      float gamma = static_cast<float>(sideAB) / orientation;

      float depth = 1.0f / (alpha / zA + beta / zB + gamma / zC);
      
      int bufferIndex = y * width + x;
      if (depth < this->depthBuffer[bufferIndex]) {
        depthBuffer[bufferIndex] = depth;
        setPixel(x, y, color);
      }
    }
  }
}
