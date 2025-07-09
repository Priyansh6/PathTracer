#include "Colour.h"

#include <algorithm>
#include <cmath>
#include <ostream>

namespace {
double linear_to_gamma(const double linear_component)
{
  if (linear_component > 0) { return std::sqrt(linear_component); }
  return 0;
}
}// namespace

void colour::write_colour(std::ostream& out, const Colour& pixel_colour)
{
  auto r = pixel_colour.x();
  auto g = pixel_colour.y();
  auto b = pixel_colour.z();

  // Apply a linear to gamma transform for gamma 2
  r = linear_to_gamma(r);
  g = linear_to_gamma(g);
  b = linear_to_gamma(b);

  const int r_byte = static_cast<int>(colour_range * std::clamp(r, 0.0, 0.999));
  const int g_byte = static_cast<int>(colour_range * std::clamp(g, 0.0, 0.999));
  const int b_byte = static_cast<int>(colour_range * std::clamp(b, 0.0, 0.999));

  std::println(out, "{} {} {}", r_byte, g_byte, b_byte);
}