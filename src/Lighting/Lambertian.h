#pragma once

#include "../../Math/Vec3.h"
#include <algorithm>

inline float calculateDiffuseIntensity (const Vec3& normal, const Vec3& lightDirection) {
  return std::max(0.0f, normal.dot(lightDirection));
}
