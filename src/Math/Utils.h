#pragma once

constexpr float PI = 3.14159265358979323846f;

constexpr float toRadians(float degrees) {
    return degrees * PI / 180.0f;
}

constexpr float toDegrees(float radians) {
    return radians * 180.0f / PI;
}
