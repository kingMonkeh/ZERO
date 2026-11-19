#include "Vec3.h"

Vec3 Vec3::operator+(const Vec3& other) const {
  return Vec3{this->x + other.x, this->y + other.y, this->z + other.z};
}

Vec3 Vec3::operator-(const Vec3& other) const {
  return Vec3{this->x - other.x, this->y - other.y, this->z - other.z};
}

Vec3 Vec3::operator*(float scalar) const {
  return Vec3{scalar * this->x, scalar * this->y, scalar * this->z};
}

Vec3 Vec3::operator/(float scalar) const {
  return Vec3{this->x / scalar, this->y / scalar, this->z / scalar};
}
