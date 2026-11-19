#pragma once

#include "Vec3.h"
#include "Utils.h"
#include <cmath>

inline Vec3 rotateY(const Vec3& vertex, float angle) {
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);

    return {
        vertex.x * cosAngle + vertex.z * sinAngle,
        vertex.y,
        -vertex.x * sinAngle + vertex.z * cosAngle
    };
}

inline Vec3 translate(const Vec3& vertex, const Vec3& offset) {
    return vertex + offset;
}
