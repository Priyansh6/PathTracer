#include "Utils.h"

#include <cmath>
#include <numbers>
#include <random>
#include <utility>

namespace {
// Function to generate random numbers
std::default_random_engine& get_generator()
{
  thread_local std::default_random_engine generator(std::random_device{}());
  return generator;
}

// Function to generate a uniform distribution
std::uniform_real_distribution<>& get_distribution()
{
  thread_local std::uniform_real_distribution distribution(0.0, 1.0);
  return distribution;
}
}// namespace

double utils::degrees_to_radians(const double degrees)
{
  return degrees * std::numbers::pi / 180.0;// NOLINT(*-magic-numbers)
}

double utils::random_double() { return get_distribution()(get_generator()); }

double utils::random_double(const double min, const double max)
{
  // Returns a random real in [min,max).
  return min + ((max - min) * random_double());
}

std::pair<double, double> utils::random_in_unit_disk()
{
  while (true) {
    const double x = random_double(-1, 1);
    const double y = random_double(-1, 1);
    if (std::sqrt((x * x) + (y * y)) < 1) { return { x, y }; }
  }
}