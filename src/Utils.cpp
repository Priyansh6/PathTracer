#include "Utils.h"

#include <numbers>
#include <random>

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

constexpr double utils::degrees_to_radians(const double degrees)
{
  return degrees * std::numbers::pi / 180.0;// NOLINT(*-magic-numbers)
}

double utils::random_double() { return get_distribution()(get_generator()); }

double utils::random_double(const double min, const double max)
{
  // Returns a random real in [min,max).
  return min + ((max - min) * random_double());
}