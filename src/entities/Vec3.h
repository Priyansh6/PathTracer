#ifndef VEC3_H
#define VEC3_H

#include "Utils.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <limits>

class Vec3;
// Point3 is just an alias for Vec3, but useful for geometric clarity in the code.
using Point3 = Vec3;

// Vec3 class represents a 3D vector or point in space.
// It provides various operations like addition, subtraction, multiplication, division,
// dot product, cross product, and vector normalization.
// It also includes methods for generating random vectors and unit vectors.
class Vec3
{
public:
  constexpr Vec3() : m_v{ 0, 0, 0 } {}
  constexpr Vec3(const double x, const double y, const double z) : m_v{ x, y, z } {}

  [[nodiscard]] constexpr double x() const { return m_v[0]; }
  [[nodiscard]] constexpr double y() const { return m_v[1]; }
  [[nodiscard]] constexpr double z() const { return m_v[2]; }

  constexpr double operator[](const size_t index) const
  {
    return m_v[index];// NOLINT(*-pro-bounds-constant-array-index)
  }
  constexpr double& operator[](const size_t index)
  {
    return m_v[index];// NOLINT(*-pro-bounds-constant-array-index)
  }

  constexpr Vec3 operator-() const { return { -m_v[0], -m_v[1], -m_v[2] }; }
  constexpr Vec3& operator+=(const Vec3& other)
  {
    m_v[0] += other.m_v[0];
    m_v[1] += other.m_v[1];
    m_v[2] += other.m_v[2];
    return *this;
  }
  constexpr Vec3& operator*=(const Vec3& other)
  {
    m_v[0] *= other.m_v[0];
    m_v[1] *= other.m_v[1];
    m_v[2] *= other.m_v[2];
    return *this;
  }
  constexpr Vec3& operator*=(const double val)
  {
    m_v[0] *= val;
    m_v[1] *= val;
    m_v[2] *= val;
    return *this;
  }
  constexpr Vec3& operator/=(const double val) { return *this *= 1 / val; }

  constexpr Vec3 operator+(const Vec3& other) const
  {
    return { m_v[0] + other.m_v[0], m_v[1] + other.m_v[1], m_v[2] + other.m_v[2] };
  }
  constexpr Vec3 operator-(const Vec3& other) const
  {
    return { m_v[0] - other.m_v[0], m_v[1] - other.m_v[1], m_v[2] - other.m_v[2] };
  }
  constexpr Vec3 operator*(const Vec3& other) const
  {
    return { m_v[0] * other.m_v[0], m_v[1] * other.m_v[1], m_v[2] * other.m_v[2] };
  }
  constexpr friend Vec3 operator*(const Vec3& vec, const double val)
  {
    return { vec.m_v[0] * val, vec.m_v[1] * val, vec.m_v[2] * val };
  }
  constexpr friend Vec3 operator*(const double val, const Vec3& vec)
  {
    return { vec.m_v[0] * val, vec.m_v[1] * val, vec.m_v[2] * val };
  }
  constexpr Vec3 operator/(const double val) const { return { m_v[0] / val, m_v[1] / val, m_v[2] / val }; }

  [[nodiscard]] double length() const { return std::sqrt(length_squared()); }
  [[nodiscard]] constexpr double length_squared() const
  {
    return (m_v[0] * m_v[0]) + (m_v[1] * m_v[1]) + (m_v[2] * m_v[2]);
  }
  [[nodiscard]] constexpr double dot(const Vec3& other) const
  {
    return (m_v[0] * other.m_v[0]) + (m_v[1] * other.m_v[1]) + (m_v[2] * other.m_v[2]);
  }
  [[nodiscard]] constexpr Vec3 cross(const Vec3& other) const
  {
    return { (m_v[1] * other.m_v[2]) - (m_v[2] * other.m_v[1]),
      (m_v[2] * other.m_v[0]) - (m_v[0] * other.m_v[2]),
      (m_v[0] * other.m_v[1]) - (m_v[1] * other.m_v[0]) };
  }
  [[nodiscard]] constexpr Vec3 unit_vector() const { return *this / length(); }
  [[nodiscard]] bool near_zero() const
  {
    return (std::abs(m_v[0]) < std::numeric_limits<double>::epsilon())
           && (std::abs(m_v[1]) < std::numeric_limits<double>::epsilon())
           && (std::abs(m_v[2]) < std::numeric_limits<double>::epsilon());
  }
  [[nodiscard]] constexpr Vec3 reflect(const Vec3& normal) const { return *this - (2 * this->dot(normal) * normal); }
  // Precondition: this and the normal are both unit vectors.
  [[nodiscard]] constexpr Vec3 refract(const Vec3& normal, const double etai_over_etat) const
  {
    const auto cos_theta = std::min(-(this->dot(normal)), 1.0);
    const Vec3 r_out_perp = etai_over_etat * (*this + cos_theta * normal);
    const Vec3 r_out_parallel = -std::sqrt(std::abs(1.0 - r_out_perp.length_squared())) * normal;
    return r_out_perp + r_out_parallel;
  }

  static Vec3 random() { return { utils::random_double(), utils::random_double(), utils::random_double() }; }
  static Vec3 random(const double min, const double max)
  {
    return { utils::random_double(min, max), utils::random_double(min, max), utils::random_double(min, max) };
  }
  static Vec3 random_unit_vector()
  {
    while (true) {
      constexpr double min_len_squared = 1e-160;
      const Vec3 p = random(-1, 1);
      if (const double len_sq = p.length_squared(); min_len_squared < len_sq && len_sq <= 1) {
        return p / sqrt(len_sq);
      }
    }
  }
  static Vec3 random_facing_normal(const Vec3& normal)
  {
    if (const Vec3 on_unit_sphere = random_unit_vector(); on_unit_sphere.dot(normal) > 0.0) {
      return on_unit_sphere;// In the same direction as the normal.
    } else {
      return -on_unit_sphere;// In the opposite direction to the normal.
    }
  }

private:
  std::array<double, 3> m_v;
};

// Useful for debugging and logging Vec3 objects. However, this requires enabling exceptions.
// template<>
// struct std::formatter<Vec3>
// {
//   constexpr auto parse(auto& ctx)
//   {
//     auto it = ctx.begin();
//     if (it != ctx.end() && *it != '}') { throw std::format_error("Invalid format args for Vec3"); }
//     return it;
//   }
//   auto format(const Vec3& v, auto& ctx) const { return std::format_to(ctx.out(), "({},{},{})", v.x(), v.y(), v.z());
//   }
// };

#endif// VEC3_H
