#include "Renderer.h"
#include "Camera.h"
#include "Colour.h"
#include "PixelWriter.h"
#include "PpmPixelWriter.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"
#include "entities/Vec3.h"
#include "entities/World.h"
#include "materials/Scatter.h"

#include <iostream>
#include <limits>
#include <ostream>
#include <variant>

constexpr double shadow_acne_epsilon = 0.0001;// Epsilon to prevent shadow acne.

template<PixelWriter PixelWriter>
Renderer<PixelWriter>::Renderer(const int image_width,
  const int image_height,
  const Camera& camera,
  PixelWriter& pixel_writer)
  : m_image_width{ image_width }, m_image_height{ image_height }, m_camera{ camera }, m_pixel_writer(pixel_writer)
{}

template<PixelWriter PixelWriter>
void Renderer<PixelWriter>::render(const World& world, const int samples_per_pixel, const int max_depth) const
{
  m_pixel_writer.init();
  for (int y = 0; y < m_image_height; y++) {
    std::print(std::clog, "\rScan Lines remaining: {:0>5}", m_image_height - y);
    std::clog.flush();
    for (int x = 0; x < m_image_width; x++) {
      const Colour pixel_colour = sample_pixel(x, y, world, samples_per_pixel, max_depth);
      m_pixel_writer.write_pixel(x, y, pixel_colour);
    }
  }
  m_pixel_writer.cleanup();
  std::println(std::clog, "\rDone.                      ");
}

template<PixelWriter PixelWriter>
Colour Renderer<PixelWriter>::sample_pixel(const int x,
  const int y,
  const World& world,
  const int samples_per_pixel,
  const int max_depth) const
{
  Colour pixel_colour = { 0, 0, 0 };
  for (int sample = 0; sample < samples_per_pixel; sample++) {
    Ray const sample_ray = m_camera.get_ray(x, y);
    pixel_colour += trace_and_shade(sample_ray, world, max_depth);
  }
  pixel_colour /= samples_per_pixel;
  return pixel_colour;
}

template<PixelWriter PixelWriter>
Colour Renderer<PixelWriter>::trace_and_shade(const Ray& r, const World& world, const int max_depth)
{
  Colour curr_diffuse_reflection_coefficient = { 1.0, 1.0, 1.0 };
  Ray curr_ray = r;

  // Perform bounces
  for (int depth = 0; depth < max_depth; depth++) {
    HitRecord rec{};
    if (!world.hit(curr_ray, shadow_acne_epsilon, std::numeric_limits<double>::max(), rec)) {
      // Ray did not hit anything, return background colour.
      const Vec3 unit_direction = curr_ray.direction().unit_vector();
      const double a = (unit_direction.y() + 1.0) / 2;
      return curr_diffuse_reflection_coefficient
             * ((1.0 - a) * world.background_bottom_colour() + a * world.background_top_colour());
    }
    ScatterRecord s_rec{};
    if (!std::visit([&](const auto& material) { return Scatter{}(material, curr_ray, rec, s_rec); }, *rec.material)) {
      // Ray hit an object but did not scatter and was absorbed, return black.
      return colour::black;
    }
    // Ray scattered, accumulate colour.
    curr_diffuse_reflection_coefficient *= s_rec.attenuation;
    curr_ray = s_rec.scattered;
  }
  // Return black if max depth reached without being absorbed.
  return colour::black;
}

template class Renderer<PpmPixelWriter>;