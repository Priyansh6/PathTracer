#ifndef CAMERA_H
#define CAMERA_H

#include "entities/Ray.h"
#include "entities/Vec3.h"

// Camera class represents a perspective camera in a 3D scene.
// It is responsible for generating rays that simulate the camera view.
// The camera can also simulate defocus by adding a random offset to the rays.
class Camera
{
public:
  Camera() = delete;
  explicit Camera(int image_width,
    int image_height,
    double vertical_fov,
    const Point3& look_from,
    const Point3& look_at,
    const Vec3& vup,
    double defocus_angle,
    double focus_dist);

  [[nodiscard]] Ray get_ray(int x, int y) const;

private:
  [[nodiscard]] Point3 defocus_disk_sample() const;

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
