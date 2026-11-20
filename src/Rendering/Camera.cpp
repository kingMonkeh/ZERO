#include "Camera.h"
#include "../Math/Transform.h"
#include "../Math/Utils.h"
#include <cmath>

Camera::Camera(float fov, Vec3 position, Vec3 rotation) : fov(fov), position{position}, rotation{rotation} {}

Vec2 Camera::project(const Vec3& point, int viewportWidth, int viewportHeight) const {
  float fovRadians = toRadians(fov);
  float focalLength = (viewportHeight / 2.0f) / std::tan(fovRadians / 2.0f);

  float t = focalLength / point.z;
  Vec3 intersectionPoint {t * point.x, t * point.y, t * point.z};

  float screenX = viewportWidth / 2.0f + intersectionPoint.x;
  float screenY = viewportHeight / 2.0f - intersectionPoint.y;

  return Vec2{screenX, screenY};
}

// The idea is we always want the camera to be at (0, 0, 0) and facing towards +Z
// So we calculate a transformed version of the world relative to the camera
Vec3 Camera::worldToCamera(const Vec3& worldPoint) const {
  Vec3 relative = worldPoint - position;

  // Perform rotations in inverse order
  Vec3 rotated = rotateZ(relative, -rotation.z);
  rotated = rotateY(rotated, -rotation.y);
  rotated = rotateX(rotated, -rotation.x);

  return rotated;
}
