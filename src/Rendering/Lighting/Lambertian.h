#pragma once

#include "../../Math/Vec3.h"
#include <algorithm>

inline float calculateDiffuseIntensity (const Vec3& normal, const Vec3& lightDirection) {
  return std::max(0.0f, normal.dot(lightDirection));
}

inline Color calculateLambertianColor (const Color& albedo, const Color& lightColor, float intensity) {
  return {
    static_cast<uint8_t>(albedo.r * (lightColor.r / 255.0f) * intensity),
    static_cast<uint8_t>(albedo.g * (lightColor.g / 255.0f) * intensity),
    static_cast<uint8_t>(albedo.b * (lightColor.b / 255.0f) * intensity)
  };
}
