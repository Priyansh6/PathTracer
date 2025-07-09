#include "Camera.h"
#include "Utils.h"
#include "entities/Ray.h"
#include "entities/Vec3.h"

#include <cmath>

Camera::Camera(const int image_width,
  const int image_height,
  const double vertical_fov,
  const Point3& look_from,
  const Point3& look_at,
  const Vec3& vup,
  const double defocus_angle,
  const double focus_dist)
{
  // NOLINTBEGIN(*-prefer-member-initializer)
  m_centre = look_from;
  const double theta = utils::degrees_to_radians(vertical_fov);
  const double h = std::tan(theta / 2);
  const double viewport_height = 2 * h * focus_dist;
  const double aspect_ratio = static_cast<double>(image_width) / static_cast<double>(image_height);
  const double viewport_width = viewport_height * aspect_ratio;

  const Vec3 w = (look_from - look_at).unit_vector();
  const Vec3 u = vup.cross(w).unit_vector();
  const Vec3 v = w.cross(u);

  const Vec3 viewport_u = viewport_width * u;
  const Vec3 viewport_v = viewport_height * -v;

  m_pixel_delta_u = viewport_u / image_width;
  m_pixel_delta_v = viewport_v / image_height;

  const Point3 viewport_upper_left = m_centre - (focus_dist * w) - (viewport_u / 2) - (viewport_v / 2);
  m_pixel00_loc = viewport_upper_left + ((m_pixel_delta_u + m_pixel_delta_v) / 2);

  // Calculate the camera defocus disk basis vectors.
  const double defocus_radius = focus_dist * std::tan(utils::degrees_to_radians(defocus_angle / 2));
  m_defocus_angle = defocus_angle;
  m_defocus_disk_u = u * defocus_radius;
  m_defocus_disk_v = v * defocus_radius;
  // NOLINTEND(*-prefer-member-initializer)
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