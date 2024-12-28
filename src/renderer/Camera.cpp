#include "Camera.h"
#include "Colour.h"
#include "Utils.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"
#include "entities/Vec3.h"
#include "entities/World.h"
#include "materials/Scatter.h"

#include <iostream>
#include <limits>
#include <ostream>
#include <print>
#include <variant>

constexpr double default_viewport_height = 2.0;
constexpr double default_focal_length = 1.0;
constexpr Point3 default_camera_centre = { 0, 0, 0 };

constexpr Colour background_start_colour = { 1.0, 1.0, 1.0 };
constexpr Colour background_end_colour = { 0.5, 0.7, 1.0 };

constexpr double shadow_acne_epsilon = 0.0001;// Epsilon to prevent shadow acne.

Camera::Camera(const double aspect_ratio, const int image_width, const int samples_per_pixel, const int max_depth)
  : m_aspect_ratio(aspect_ratio), m_image_width(image_width),
    m_image_height(static_cast<int>(image_width / aspect_ratio)), m_samples_per_pixel(samples_per_pixel),
    m_max_depth(max_depth), m_viewport_height(default_viewport_height), m_focal_length(default_focal_length),
    m_camera_centre(default_camera_centre)
{
  const double actual_aspect_ratio = static_cast<double>(image_width) / static_cast<double>(m_image_height);

  m_viewport_width = m_viewport_height * actual_aspect_ratio;

  m_viewport_u = { m_viewport_width, 0, 0 };
  m_viewport_v = { 0, -m_viewport_height, 0 };

  m_pixel_delta_u = m_viewport_u / image_width;
  m_pixel_delta_v = m_viewport_v / m_image_height;

  m_viewport_upper_left = m_camera_centre - Vec3(0, 0, m_focal_length) - (m_viewport_u / 2) - (m_viewport_v / 2);
  m_pixel00_loc = m_viewport_upper_left + ((m_pixel_delta_u + m_pixel_delta_v) / 2);
}

void Camera::render(const World& world) const
{
  std::println("P3\n{} {}\n{}", m_image_width, m_image_height, colour::colour_range - 1);

  for (int y = 0; y < m_image_height; y++) {
    std::print(std::clog, "\rScan Lines remaining: {:0>5}", m_image_height - y);
    std::clog.flush();
    for (int x = 0; x < m_image_width; x++) {
      Colour pixel_colour = { 0, 0, 0 };
      for (int sample = 0; sample < m_samples_per_pixel; sample++) {
        Ray const sample_ray = get_ray(x, y);
        pixel_colour += ray_colour(sample_ray, world);
      }
      pixel_colour /= m_samples_per_pixel;
      write_colour(std::cout, pixel_colour);
    }
  }
  std::println(std::clog, "\rDone.                      ");
}

Colour Camera::ray_colour(const Ray& r, const World& world) const
{
  HitRecord rec{};
  Colour curr_diffuse_reflection_coefficient = { 1.0, 1.0, 1.0 };
  Ray curr_ray = r;
  int depth = m_max_depth;

  // Perform bounces
  while (world.hit(curr_ray, shadow_acne_epsilon, std::numeric_limits<double>::infinity(), rec)) {
    if (ScatterRecord s_rec{};
      std::visit([&](const auto& material) { return Scatter{}(material, curr_ray, rec, s_rec); }, *rec.material)) {
      curr_diffuse_reflection_coefficient *= s_rec.attenuation;
      curr_ray = s_rec.scattered;
    } else {
      return { 0, 0, 0 };
    }

    // If we've reached the maximum depth, return black.
    if (--depth == 0) { return { 0, 0, 0 }; }
  }

  // Background colour
  const Vec3 unit_direction = r.direction().unit_vector();
  const double a = (unit_direction.y() + 1.0) / 2;
  return curr_diffuse_reflection_coefficient * ((1.0 - a) * background_start_colour + a * background_end_colour);
}

Ray Camera::get_ray(const int x, const int y) const
{
  // Construct a camera ray originating from the origin and directed at randomly sampled
  // point around the pixel location x, y.
  const Point3 pixel_sample = m_pixel00_loc + ((x + utils::random_double() - 0.5) * m_pixel_delta_u)
                              + ((y + utils::random_double() - 0.5) * m_pixel_delta_v);
  const Vec3 ray_direction = pixel_sample - m_camera_centre;

  return Ray(m_camera_centre, ray_direction);
}