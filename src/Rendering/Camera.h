#pragma once

#include "../Math/Vec2.h"
#include "../Math/Vec3.h"

class Camera {
  public:
    Camera(float fov, Vec3 position, Vec3 rotation);

    Vec2 project(const Vec3& point, int viewportWidth, int viewportHeight) const;
    Vec3 worldToCamera(const Vec3& worldPoint) const;

  private:
    float fov;
    Vec3 position;
    Vec3 rotation;
};
