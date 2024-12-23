#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"

class Sphere
{
public:
  constexpr Sphere(const Point3& centre, const double radius) : m_centre(centre), m_radius(std::max(0.0, radius)) {}

  bool hit(const Ray& r, double min_t, double max_t, HitRecord& rec) const;

private:
  Point3 m_centre;
  double m_radius;
};

#endif// SPHERE_H
