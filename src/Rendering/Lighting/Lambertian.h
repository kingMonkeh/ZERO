#pragma once

#include "Lighting.h"
#include "../../Geometry/Scene.h"
#include "../../Math/Vec3.h"
#include <algorithm>

inline float calculateDiffuseIntensity (const Vec3& normal, const Vec3& lightDirection) {
  return std::max(0.0f, normal.dot(lightDirection));
}

inline Color applyIllumination (const Color& albedo, const Illumination& illumination) {
  return {
    static_cast<uint8_t>(std::min(255.0f, albedo.r * illumination.r)),
    static_cast<uint8_t>(std::min(255.0f, albedo.g * illumination.g)),
    static_cast<uint8_t>(std::min(255.0f, albedo.b * illumination.b))
  };
}

inline Illumination calculateIllumination (const Vec3& normal, const Scene& scene) {
  Illumination result;

  // Ambient light
  result.r += (scene.ambientLight.color.r / 255.0f) * scene.ambientLight.intensity;
  result.g += (scene.ambientLight.color.g / 255.0f) * scene.ambientLight.intensity;
  result.b += (scene.ambientLight.color.b / 255.0f) * scene.ambientLight.intensity;

  // Directional lights
  for (const DirectionalLight& light : scene.directionalLights) {
    float intensity = calculateDiffuseIntensity(normal, light.direction);

    result.r += (light.color.r / 255.0f) * intensity;
    result.g += (light.color.g / 255.0f) * intensity;
    result.b += (light.color.b / 255.0f) * intensity;
  }

  return result;
}
