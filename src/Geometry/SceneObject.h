#pragma once

#include "Mesh.h"
#include "../Math/Vec3.h"

struct SceneObject {
    Mesh mesh;
    Vec3 position;
    float rotationY;
};
