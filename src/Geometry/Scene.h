#pragma once

#include <vector>

#include "SceneObject.h"
#include "../Rendering/Lighting/Lighting.h"

struct Scene {
    std::vector<SceneObject> objects;
    std::vector<DirectionalLight> directionalLights;
};
