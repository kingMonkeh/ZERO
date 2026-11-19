#include <algorithm>
#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : 
  width(width), 
  height(height), 
  pixels(width * height) {}

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
    int x0, int y0,
    int x1, int y1,
    int x2, int y2,
    Color color
) {

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

      if (inside) {
        setPixel(x, y, color);
      }
    }
  }
}
