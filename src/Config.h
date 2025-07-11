#ifndef CONFIG_H
#define CONFIG_H

#include "entities/Vec3.h"

#include <string>

namespace app_metadata {
const std::string name = "Path Tracer";// NOLINT(*-err58-cpp)
const std::string version = "1.0";// NOLINT(*-err58-cpp)
const std::string id = "path-tracer";// NOLINT(*-err58-cpp)
}// namespace app_metadata

namespace window_config {
const std::string title = "Path Tracer";// NOLINT(*-err58-cpp)
constexpr int default_width = 1200;// Default width of the window
constexpr double aspect_ratio = 16.0 / 9.0;// Ratio of width to height
constexpr int default_tile_size = 16;// Size of each (square) tile in pixels
constexpr int min_frame_time_ms = 16;// Minimum frame time in milliseconds (60 FPS)
}// namespace window_config

namespace camera_config {
constexpr double vertical_fov = 20;// Vertical field of view in degrees
constexpr Point3 look_from = { 13, 2, 3 };// Camera position
constexpr Point3 look_at = { 0, 0, 0 };// Point the camera is looking at
constexpr Vec3 vup = { 0, 1, 0 };// Up vector
constexpr double defocus_angle = 0.6;// Variation angle of rays through each pixel
constexpr double focus_dist = 10;// Distance from camera look_from point to plane of perfect focus
}// namespace camera_config

namespace rt_config {
constexpr int default_samples_per_pixel = 500;// Number of samples per pixel
constexpr int default_max_depth = 50;// Maximum bounces per ray
}// namespace rt_config

#endif// CONFIG_H
