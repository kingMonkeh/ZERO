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
