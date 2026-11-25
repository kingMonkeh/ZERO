#pragma once

#include "Mesh.h"
#include "../Math/Vec3.h"
#include "../Rendering/Material.h"

struct SceneObject {
    Mesh mesh;
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    Material material;
};
