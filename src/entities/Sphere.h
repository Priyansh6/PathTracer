#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"
#include "materials/Material.h"

#include <algorithm>

// Sphere class represents a sphere in 3D space.
// It is defined by its centre, radius, and material.
// The class provides a method to check if a ray intersects the sphere,
// returning a HitRecord containing intersection details if it does.
class Sphere
{
public:
  constexpr Sphere(const Point3& centre, const double radius, const Material& material)
    : m_centre{ centre }, m_radius{ std::max(0.0, radius) }, m_material{ material }
  {}

  bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const;
  [[nodiscard]] Point3 centre() const { return m_centre; }
  [[nodiscard]] double radius() const { return m_radius; }

private:
  Point3 m_centre;
  double m_radius;
  Material m_material;
};

#endif// SPHERE_H
