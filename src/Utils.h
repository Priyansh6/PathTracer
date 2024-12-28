#ifndef UTILS_H
#define UTILS_H

#include <ranges>
#include <utility>

namespace utils {
template<class T, class V>
concept input_range_of = std::ranges::input_range<T> && std::is_same_v<V, std::ranges::range_value_t<T>>;

double degrees_to_radians(double degrees);
// Returns a random real in [0,1).
double random_double();
// Returns a random real in [min,max).
double random_double(double min, double max);
// Returns a random point in the unit disk.
std::pair<double, double> random_in_unit_disk();
}// namespace utils

#endif// UTILS_H
