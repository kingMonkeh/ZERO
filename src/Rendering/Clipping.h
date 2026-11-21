#pragma once

#include <vector>
#include "../Geometry/Mesh.h"

std::vector<Triangle3D> clipTriangleAgainstNearPlane(const Triangle3D& triangle, float nearPlane);
