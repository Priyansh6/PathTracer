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
  explicit Camera(double aspect_ratio, int image_width, int samples_per_pixel);
  void render(const World& world) const;

private:
  static Colour ray_colour(const Ray& r, const World& world);
  [[nodiscard]] Ray get_ray(int x, int y) const;

  // Camera configuration.
  double m_aspect_ratio;
  int m_image_width;
  int m_image_height;
  int m_samples_per_pixel;

  // Viewport configuration.
  double m_viewport_height;
  double m_viewport_width;

  // Camera position.
  double m_focal_length;
  Point3 m_camera_centre;

  // Viewport vectors and points
  Vec3 m_viewport_u;
  Vec3 m_viewport_v;
  Vec3 m_pixel_delta_u;
  Vec3 m_pixel_delta_v;
  Point3 m_viewport_upper_left;
  Point3 m_pixel00_loc;
};

#endif// CAMERA_H
