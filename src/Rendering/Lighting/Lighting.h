#pragma once

#include "../../Math/Vec3.h"
#include "../Color.h"

struct DirectionalLight {
    Vec3 direction;
    Color color;
};
