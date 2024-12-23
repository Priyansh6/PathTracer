#include "World.h"
#include "Hittable.h"
#include "Ray.h"
#include "Sphere.h"

bool World::hit(const Ray& r, const double t_min, const double t_max, HitRecord& rec) const
{
  return hit<Sphere>(m_spheres, r, t_min, t_max, rec);
}