#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

// Ray class represents a ray in 3D space.
// It is defined by an origin point and a direction vector.
// The Ray class provides methods to compute points along the ray at a given distance.
class Ray
{
public:
  Ray() = default;
  explicit Ray(const Point3& origin, const Vec3& direction);

  [[nodiscard]] const Point3& origin() const;
  [[nodiscard]] const Vec3& direction() const;

  [[nodiscard]] Point3 at(double t) const;

private:
  Point3 m_origin;
  Vec3 m_direction;
};

#endif// RAY_H
