
#pragma once

#include "Mesh.h"
#include <cmath>
#include "../Math/Utils.h"

inline Mesh createSphere(int segments = 32, int rings = 16) {
    Mesh sphere;

    // Generate vertices
    for (int i = 0; i <= rings; i++) {
        float phi = PI * static_cast<float>(i) / rings;

        for (int j = 0; j <= segments; j++) {
            float theta = 2.0f * PI * static_cast<float>(j) / segments;

            float x = std::sin(phi) * std::cos(theta);
            float y = std::cos(phi);
            float z = std::sin(phi) * std::sin(theta);

            sphere.vertices.push_back({x, y, z});
        }
    }

    // Generate triangles
    for (int i = 0; i < rings; i++) {
        for (int j = 0; j < segments; j++) {
            int current = i * (segments + 1) + j;
            int next = current + segments + 1;

            sphere.triangles.push_back({current, current + 1, next});
            sphere.triangles.push_back({current + 1, next + 1, next});
        }
    }

    return sphere;
}
