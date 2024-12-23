#ifndef UTILS_H
#define UTILS_H

#include <numbers>
#include <ranges>

namespace utils {
constexpr double degrees_to_radians(const double degrees)
{
  return degrees * std::numbers::pi / 180.0;// NOLINT(*-magic-numbers)
}

template<class T, class V>
concept input_range_of = std::ranges::input_range<T> && std::is_same_v<V, std::ranges::range_value_t<T>>;
}// namespace utils

#endif// UTILS_H
