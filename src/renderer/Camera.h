#ifndef CAMERA_H
#define CAMERA_H

#include "Colour.h"
#include "entities/Ray.h"
#include "entities/Vec3.h"
#include "entities/World.h"

class Camera
{
public:
  Camera() = delete;
  explicit Camera(double aspect_ratio,
    int image_width,
    int samples_per_pixel,
    int max_depth,
    double vertical_fov,
    const Point3& look_from,
    const Point3& look_at,
    const Vec3& vup,
    double defocus_angle,
    double focus_dist);
  void render(const World& world) const;

private:
  [[nodiscard]] Colour ray_colour(const Ray& r, const World& world) const;
  [[nodiscard]] Ray get_ray(int x, int y) const;
  [[nodiscard]] Point3 defocus_disk_sample() const;

  // Camera configuration.
  int m_image_width;
  int m_image_height;
  int m_samples_per_pixel;
  int m_max_depth;

  // Camera position.
  Point3 m_centre;

  // Viewport vectors and points
  Vec3 m_pixel_delta_u;
  Vec3 m_pixel_delta_v;
  Point3 m_pixel00_loc;
  double m_defocus_angle;
  Vec3 m_defocus_disk_u;
  Vec3 m_defocus_disk_v;
};

#endif// CAMERA_H
