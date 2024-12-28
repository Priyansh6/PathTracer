#include "World.h"
#include "Hittable.h"
#include "Ray.h"
#include "Sphere.h"

bool World::hit(const Ray& r, const double t_min, const double t_max, HitRecord& rec) const
{
  HitRecord temp_rec{};
  auto closest_so_far = t_max;

  for (const Sphere& sphere : m_spheres) {
    if (sphere.hit(r, t_min, closest_so_far, temp_rec)) {
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return closest_so_far < t_max;
}