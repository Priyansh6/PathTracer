#ifndef CONFIG_H
#define CONFIG_H

namespace config {
constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int image_width = 400;
constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
}// namespace config

#endif// CONFIG_H
