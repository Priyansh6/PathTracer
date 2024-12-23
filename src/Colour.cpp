#include "Colour.h"

#include <ostream>

constexpr double colour_multiplier = colour::maximum_value + 0.999;

void write_colour(std::ostream& out, const Colour& pixel_colour)
{
  const auto r = pixel_colour.x();
  const auto g = pixel_colour.y();
  const auto b = pixel_colour.z();

  const int r_byte = static_cast<int>(colour_multiplier * r);
  const int g_byte = static_cast<int>(colour_multiplier * g);
  const int b_byte = static_cast<int>(colour_multiplier * b);

  std::println(out, "{} {} {}", r_byte, g_byte, b_byte);
}