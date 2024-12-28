#ifndef CONFIG_H
#define CONFIG_H

#include "entities/Vec3.h"

namespace config {
// Image configuration
constexpr double aspect_ratio = 16.0 / 9.0;// Ratio of width to height
constexpr int image_width = 1200;// Image width in pixels

// Camera configuration
constexpr double vertical_fov = 20;// Vertical field of view in degrees
constexpr Point3 look_from = { 13, 2, 3 };// Camera position
constexpr Point3 look_at = { 0, 0, 0 };// Point the camera is looking at
constexpr Vec3 vup = { 0, 1, 0 };// Up vector
constexpr double defocus_angle = 0.6;// Variation angle of rays through each pixel
constexpr double focus_dist = 10;// Distance from camera look_from point to plane of perfect focus

// Rendering configuration
constexpr int samples_per_pixel = 500;// Number of samples per pixel
constexpr int max_depth = 50;// Maximum bounces per ray
}// namespace config

#endif// CONFIG_H
