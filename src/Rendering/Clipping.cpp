#include "Clipping.h"

static bool visible (const Vec3& point, float nearPlane) {
  return point.z >= nearPlane;
}

static Vec3 calculateIntersection (const Vec3& a, const Vec3& b, float nearPlane) {
  Vec3 edgeAB = b - a;
  float t = (nearPlane - a.z) / (b.z - a.z);
  return a + edgeAB * t;
}

std::vector<Triangle3D> clipTriangleAgainstNearPlane(const Triangle3D& triangle, float nearPlane) {

  std::vector<Vec3> vertices = {
    triangle.a,
    triangle.b,
    triangle.c
  };

  std::vector<Vec3> clippedVertices;

  for (int i = 0; i < 3; i++) {
    const Vec3& a = vertices[i];
    const Vec3& b = vertices[(i + 1) % 3];

    bool aSeen = visible(a, nearPlane);
    bool bSeen = visible(b, nearPlane);

    if (aSeen && bSeen) {
      clippedVertices.push_back(b);
    }
    else if (aSeen && !bSeen) {
      clippedVertices.push_back(calculateIntersection(a, b, nearPlane));
    }
    else if (!aSeen && bSeen) {
      clippedVertices.push_back(calculateIntersection(a, b, nearPlane));
      clippedVertices.push_back(b);
    }
  }

  std::vector<Triangle3D> result;

  // Order of clippedVertices is important
  for (size_t i = 1; i + 1 < clippedVertices.size(); i++) {
    result.push_back({
      clippedVertices[0],
      clippedVertices[i],
      clippedVertices[i + 1]
    });
  }

  return result;

}
