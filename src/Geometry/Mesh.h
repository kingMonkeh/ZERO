#pragma once

#include <vector>
#include "../Math/Vec3.h"

struct Triangle {
  int a;
  int b;
  int c;
};

struct Mesh {
  std::vector<Vec3> vertices;
  std::vector<Triangle> triangles;
};
