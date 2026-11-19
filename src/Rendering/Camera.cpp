#include "Camera.h"
#include "../Math/Utils.h"
#include <cmath>

Camera::Camera(float fov) : fov(fov) {}

Vec2 Camera::project(const Vec3& point, int viewportWidth, int viewportHeight) const {
  float fovRadians = toRadians(fov);
  float focalLength = (viewportHeight / 2.0f) / std::tan(fovRadians / 2.0f);

  float t = focalLength / point.z;
  Vec3 intersectionPoint {t * point.x, t * point.y, t * point.z};

  float screenX = viewportWidth / 2.0f + intersectionPoint.x;
  float screenY = viewportHeight / 2.0f - intersectionPoint.y;

  return Vec2{screenX, screenY};
}
