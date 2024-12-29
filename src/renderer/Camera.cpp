#include "Camera.h"
#include "Colour.h"
#include "Utils.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"
#include "entities/Vec3.h"
#include "entities/World.h"
#include "materials/Scatter.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <ostream>
#include <print>
#include <variant>

constexpr double shadow_acne_epsilon = 0.0001;// Epsilon to prevent shadow acne.

Camera::Camera(const double aspect_ratio,
  const int image_width,
  const int samples_per_pixel,
  const int max_depth,
  const double vertical_fov,
  const Point3& look_from,
  const Point3& look_at,
  const Vec3& vup,
  const double defocus_angle,
  const double focus_dist)
{
  // NOLINTBEGIN(*-prefer-member-initializer)
  m_image_width = image_width;
  m_image_height = static_cast<int>(image_width / aspect_ratio);
  m_samples_per_pixel = samples_per_pixel;
  m_max_depth = max_depth;
  m_centre = look_from;
  const double theta = utils::degrees_to_radians(vertical_fov);
  const double h = std::tan(theta / 2);
  const double viewport_height = 2 * h * focus_dist;
  const double actual_aspect_ratio = static_cast<double>(image_width) / static_cast<double>(m_image_height);
  const double viewport_width = viewport_height * actual_aspect_ratio;

  const Vec3 w = (look_from - look_at).unit_vector();
  const Vec3 u = vup.cross(w).unit_vector();
  const Vec3 v = w.cross(u);

  const Vec3 viewport_u = viewport_width * u;
  const Vec3 viewport_v = viewport_height * -v;

  m_pixel_delta_u = viewport_u / image_width;
  m_pixel_delta_v = viewport_v / m_image_height;

  const Point3 viewport_upper_left = m_centre - (focus_dist * w) - (viewport_u / 2) - (viewport_v / 2);
  m_pixel00_loc = viewport_upper_left + ((m_pixel_delta_u + m_pixel_delta_v) / 2);

  // Calculate the camera defocus disk basis vectors.
  const double defocus_radius = focus_dist * std::tan(utils::degrees_to_radians(defocus_angle / 2));
  m_defocus_angle = defocus_angle;
  m_defocus_disk_u = u * defocus_radius;
  m_defocus_disk_v = v * defocus_radius;
  // NOLINTEND(*-prefer-member-initializer)
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
  while (world.hit(curr_ray, shadow_acne_epsilon, std::numeric_limits<double>::max(), rec)) {
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
  return curr_diffuse_reflection_coefficient
         * ((1.0 - a) * world.background_bottom_colour() + a * world.background_top_colour());
}

// Construct a camera ray originating from the defocus disk and directed at randomly sampled
// point around the pixel location x, y.
Ray Camera::get_ray(const int x, const int y) const
{
  const Point3 pixel_sample = m_pixel00_loc + ((x + utils::random_double() - 0.5) * m_pixel_delta_u)
                              + ((y + utils::random_double() - 0.5) * m_pixel_delta_v);
  const Point3 ray_origin = (m_defocus_angle <= 0) ? m_centre : defocus_disk_sample();
  const Vec3 ray_direction = pixel_sample - ray_origin;

  return Ray(ray_origin, ray_direction);
}

Point3 Camera::defocus_disk_sample() const
{
  // Returns a random point in the camera defocus disk.
  auto [x, y] = utils::random_in_unit_disk();
  return m_centre + (x * m_defocus_disk_u) + (y * m_defocus_disk_v);
}