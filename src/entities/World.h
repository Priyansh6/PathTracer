#ifndef WORLD_H
#define WORLD_H

#include "Hittable.h"
#include "Ray.h"
#include "Sphere.h"
#include "Utils.h"
#include "materials/Dielectric.h"
#include "materials/Lambertian.h"
#include "renderer/Colour.h"

#include <vector>

namespace default_world {
constexpr Colour background_top_colour = { 0.5, 0.7, 1 };
constexpr Colour background_bottom_colour = { 1, 1, 1 };

inline std::vector<Sphere> generate_spheres()
{
  // NOLINTBEGIN(*-magic-numbers)
  std::vector<Sphere> spheres;
  spheres.emplace_back(Point3(0, -1000, 0), 1000, Lambertian(Colour(0.5, 0.5, 0.5)));// Ground sphere

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      const double choose_mat = utils::random_double();
      const Point3 centre(a + (0.9 * utils::random_double()), 0.2, b + (0.9 * utils::random_double()));
      if ((centre - Point3(4, 0.2, 0)).length() < 0.9) { continue; }

      if (choose_mat < 0.8) {
        // diffuse
        const Colour albedo = Colour::random() * Colour::random();
        spheres.emplace_back(centre, 0.2, Lambertian(albedo));
      } else if (choose_mat < 0.95) {
        // metal
        const Colour albedo = Colour::random(0.5, 1);
        const double fuzz = utils::random_double(0, 0.5);
        spheres.emplace_back(centre, 0.2, Metal(albedo, fuzz));
      } else {
        // glass
        spheres.emplace_back(centre, 0.2, Dielectric(1.5));
      }
    }
  }// Small marbles

  spheres.emplace_back(Point3(0, 1, 0), 1.0, Dielectric(1.5));// Large glass marble
  spheres.emplace_back(Point3(-4, 1, 0), 1.0, Lambertian(Colour(0.4, 0.2, 0.1)));// Large diffuse marble
  spheres.emplace_back(Point3(4, 1, 0), 1.0, Metal(Colour(0.7, 0.6, 0.5), 0.0));// Large metal marble

  return spheres;
  // NOLINTEND(*-magic-numbers)
}
}// namespace default_world

namespace three_spheres_world {
constexpr Colour background_top_colour = { 0.5, 0.7, 1 };
constexpr Colour background_bottom_colour = { 1, 1, 1 };

inline std::vector<Sphere> generate_spheres()
{
  // NOLINTBEGIN(*-magic-numbers)
  std::vector<Sphere> spheres;
  spheres.emplace_back(Point3(0, -100.5, -1), 100, Lambertian(Colour(0.8, 0.8, 0.0)));// Ground sphere
  spheres.emplace_back(Point3(0, 0, -1.2), 0.5, Lambertian(Colour(0.1, 0.2, 0.5)));// Centre sphere
  spheres.emplace_back(Point3(-1, 0, -1), 0.5, Dielectric(1.5));// Left sphere
  spheres.emplace_back(Point3(-1, 0, -1), 0.4, Dielectric(1.0 / 1.5));// Bubble sphere
  spheres.emplace_back(Point3(1, 0, -1), 0.5, Metal(Colour(0.8, 0.6, 0.2), 1.0));// Right sphere
  return spheres;
  // NOLINTEND(*-magic-numbers)
}
}// namespace three_spheres_world

class World
{
public:
  World() = delete;
  explicit World(const Colour& background_top_colour,
    const Colour& background_bottom_colour,
    utils::input_range_of<Sphere> auto&& spheres)
    : m_background_top_colour(background_top_colour), m_background_bottom_colour(background_bottom_colour),
      m_spheres(std::ranges::to<std::vector<Sphere>>(spheres)) {};

  bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
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
  };
  [[nodiscard]] const Colour& background_top_colour() const { return m_background_top_colour; }
  [[nodiscard]] const Colour& background_bottom_colour() const { return m_background_bottom_colour; }

private:
  Colour m_background_top_colour;
  Colour m_background_bottom_colour;
  std::vector<Sphere> m_spheres;
};

#endif// WORLD_H
