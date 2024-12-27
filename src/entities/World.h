#ifndef WORLD_H
#define WORLD_H

#include "Hittable.h"
#include "Ray.h"
#include "Sphere.h"
#include "Utils.h"
#include "materials/Dielectric.h"
#include "materials/Lambertian.h"

#include <array>
#include <vector>

namespace default_world {
constexpr std::array spheres = {
  Sphere(Point3(0, -100.5, -1), 100, Lambertian(Colour(0.8, 0.8, 0.0))),// ground
  Sphere(Point3(0, 0, -1.2), 0.5, Lambertian(Colour(0.1, 0.2, 0.5))),// center
  Sphere(Point3(-1.0, 0, -1.0), 0.5, Dielectric(1.50)),// left
  Sphere(Point3(1.0, 0, -1.0), 0.5, Metal(Colour(0.8, 0.6, 0.2), 1.0))// right
};
}// namespace default_world

class World
{
public:
  World() = delete;
  explicit World(utils::input_range_of<Sphere> auto&& spheres)
    : m_spheres(std::ranges::to<std::vector<Sphere>>(spheres)) {};

  bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const;

private:
  std::vector<Sphere> m_spheres;
};// namespace world

#endif// WORLD_H
