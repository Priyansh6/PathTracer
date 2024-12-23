#include "Colour.h"

#include <algorithm>
#include <ostream>

void write_colour(std::ostream& out, const Colour& pixel_colour)
{
  const auto r = pixel_colour.x();
  const auto g = pixel_colour.y();
  const auto b = pixel_colour.z();

  const int r_byte = static_cast<int>(colour::colour_range * std::clamp(r, 0.0, 0.999));
  const int g_byte = static_cast<int>(colour::colour_range * std::clamp(g, 0.0, 0.999));
  const int b_byte = static_cast<int>(colour::colour_range * std::clamp(b, 0.0, 0.999));

  std::println(out, "{} {} {}", r_byte, g_byte, b_byte);
}