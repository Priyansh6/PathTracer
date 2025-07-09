#include "PpmPixelWriter.h"
#include "Colour.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <ostream>

namespace {
constexpr Colour invalid_colour{ 2.0, 2.0, 2.0 };

constexpr double linear_to_gamma(const double linear_component)
{
  if (linear_component > 0) { return std::sqrt(linear_component); }
  return 0;
}

constexpr bool is_invalid_pixel(const Colour& colour)
{
  return colour.x() > 1.0 || colour.y() > 1.0 || colour.z() > 1.0;
}
}// namespace

PpmPixelWriter::PpmPixelWriter(const int width, const int height, std::ostream& out)
  : m_width{ width }, m_height{ height }, m_pixels{ static_cast<std::size_t>(width * height), invalid_colour },
    m_out{ out }
{}

// Can only be called once, before any pixels are rendered.
void PpmPixelWriter::init() { std::println(m_out, "P3\n{} {}\n{}", m_width, m_height, colour::colour_range - 1); }

void PpmPixelWriter::write_pixel(const int x, const int y, const Colour& colour)
{
  auto r = colour.x();
  auto g = colour.y();
  auto b = colour.z();

  // Apply a linear to gamma transform for gamma 2
  constexpr double max_colour_value = 0.999;
  r = std::clamp(linear_to_gamma(r), 0.0, max_colour_value);
  g = std::clamp(linear_to_gamma(g), 0.0, max_colour_value);
  b = std::clamp(linear_to_gamma(b), 0.0, max_colour_value);

  m_pixels[(y * m_width) + x] = { r, g, b };

  // Render next available pixels
  while (m_current_pixel < m_pixels.size() && !is_invalid_pixel(m_pixels[m_current_pixel])) {
    const Colour& curr_pixel = m_pixels[m_current_pixel];
    const int r_byte = static_cast<int>(colour::colour_range * curr_pixel.x());
    const int g_byte = static_cast<int>(colour::colour_range * curr_pixel.y());
    const int b_byte = static_cast<int>(colour::colour_range * curr_pixel.z());
    std::println(m_out, "{} {} {}", r_byte, g_byte, b_byte);
    m_current_pixel++;
  }
}

void PpmPixelWriter::cleanup() {}
