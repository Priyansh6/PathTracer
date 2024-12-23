#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray
{
public:
  Ray() = delete;
  explicit Ray(const Point3& origin, const Vec3& direction);

  [[nodiscard]] const Point3& origin() const;
  [[nodiscard]] const Vec3& direction() const;

  [[nodiscard]] Point3 at(double t) const;

private:
  Point3 m_origin;
  Vec3 m_direction;
};

#endif// RAY_H
