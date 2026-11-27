#pragma once

#include "../../Math/Vec3.h"
#include "../Color.h"

struct DirectionalLight {
    Vec3 direction;
    Color color;
};

struct AmbientLight {
    Color color;
    float intensity;
};

struct Illumination {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};
