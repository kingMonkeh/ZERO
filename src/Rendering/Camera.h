#pragma once

#include "../Math/Vec2.h"
#include "../Math/Vec3.h"

class Camera {
  public:
    Camera(float fov);

    Vec2 project(const Vec3& point, int viewportWidth, int viewportHeight) const;

  private:
    float fov;
};
