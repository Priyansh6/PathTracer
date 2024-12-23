#ifndef CAMERA_H
#define CAMERA_H

#include "Config.h"
#include "Vec3.h"

namespace camera {
constexpr double actual_aspect_ratio =
  static_cast<double>(config::image_width) / static_cast<double>(config::image_height);

constexpr double viewport_height = 2.0;
constexpr double viewport_width = viewport_height * actual_aspect_ratio;

constexpr double focal_length = 1.0;
constexpr Point3 camera_centre(0, 0, 0);

constexpr Vec3 viewport_u = { viewport_width, 0, 0 };
constexpr Vec3 viewport_v = { 0, -viewport_height, 0 };

constexpr Vec3 pixel_delta_u = viewport_u / config::image_width;
constexpr Vec3 pixel_delta_v = viewport_v / config::image_height;

constexpr Point3 viewport_upper_left = camera_centre - Vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
constexpr Point3 pixel00_loc = viewport_upper_left + (0.5 * (pixel_delta_u + pixel_delta_v));
}// namespace camera

#endif// CAMERA_H
