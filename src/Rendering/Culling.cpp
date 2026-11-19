#include "Culling.h"

bool isBackFacing(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& cameraPosition) {
  Vec3 normal = (b - a).cross(c - a);
  Vec3 triangleToCameraDirection = cameraPosition - a;

  return normal.dot(triangleToCameraDirection) < 0.0f;
}
