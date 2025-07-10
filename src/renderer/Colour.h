#ifndef COLOUR_H
#define COLOUR_H

#include "entities/Vec3.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <tuple>

using Colour = Vec3;

namespace colour {
// Num possibilities for a colour component.
constexpr int colour_range = 256;

constexpr Colour black{ 0, 0, 0 };
}// namespace colour

class RGBA
{
public:
  constexpr explicit RGBA(const std::uint32_t data) : m_data{ data } {}
  constexpr explicit RGBA(const Colour& colour)
  {
    const double r = std::clamp(linear_to_gamma(colour.x()), 0.0, max_colour_value);
    const double g = std::clamp(linear_to_gamma(colour.y()), 0.0, max_colour_value);
    const double b = std::clamp(linear_to_gamma(colour.z()), 0.0, max_colour_value);

    m_data = (static_cast<std::uint32_t>(r * colour::colour_range) << 24U)// NOLINT(*-magic-numbers)
             | (static_cast<std::uint32_t>(g * colour::colour_range) << 16U)// NOLINT(*-magic-numbers)
             | (static_cast<std::uint32_t>(b * colour::colour_range) << 8U)// NOLINT(*-magic-numbers)
             | component_mask;
  }

  [[nodiscard]] constexpr std::uint32_t get_value() const { return m_data; }
  [[nodiscard]] constexpr std::tuple<std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t> get_components() const
  {
    const auto r = static_cast<std::uint8_t>((m_data >> 24U) & component_mask);
    const auto g = static_cast<std::uint8_t>((m_data >> 16U) & component_mask);
    const auto b = static_cast<std::uint8_t>((m_data >> 8U) & component_mask);
    const auto a = static_cast<std::uint8_t>(m_data & component_mask);
    return { r, g, b, a };
  }

  static constexpr std::uint32_t component_mask{ 0xFFU };

private:
  std::uint32_t m_data;

  static constexpr double max_colour_value = 0.999;

  static double linear_to_gamma(const double linear_component)
  {
    if (linear_component > 0) { return std::sqrt(linear_component); }
    return 0;
  }
};

namespace rgba {
inline constexpr RGBA black_rgba{ 0xFFU };
}// namespace rgba

#endif// COLOUR_H
