#ifndef WORLD_H
#define WORLD_H

#include "Hittable.h"
#include "Ray.h"
#include "Sphere.h"
#include "Utils.h"

#include <array>
#include <vector>

namespace default_world {
constexpr std::array spheres = { Sphere(Point3(0, 0, -1), 0.5), Sphere(Point3(0, -100.5, -1), 100) };
}

class World
{
public:
  World() = delete;
  explicit World(utils::input_range_of<Sphere> auto&& spheres)
    : m_spheres(std::ranges::to<std::vector<Sphere>>(spheres))
  {}

  bool hit(const Ray& r, const double t_min, const double t_max, HitRecord& rec) const
  {
    return hit<Sphere>(m_spheres, r, t_min, t_max, rec);
  }

private:
  std::vector<Sphere> m_spheres;
};// namespace world

#endif// WORLD_H
