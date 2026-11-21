#pragma once

#include "../Math/Vec2.h"
#include "../Math/Vec3.h"

class Camera {
  public:
    Camera(float fov, float nearPlane, Vec3 position, Vec3 rotation);

    Vec2 project(const Vec3& point, int viewportWidth, int viewportHeight) const;
    Vec3 worldToCamera(const Vec3& worldPoint) const;
    void move(const Vec3& localMovement);
    void rotateBy(const Vec3& rotationDelta);
    float getNearPlane();

  private:
    float fov;
    float nearPlane;
    Vec3 position;
    Vec3 rotation;
};
