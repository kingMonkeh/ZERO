#pragma once

#include "../Math/Vec3.h"
#include "../Rendering/Color.h"

struct DirectionalLight {
    Vec3 direction;
    Color color;
};
