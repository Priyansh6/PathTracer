#include "Vec3.h"

#include <cmath>
#include <cstdlib>
#include <format>
#include <iostream>
#include <ostream>

Vec3::Vec3() : m_v{ 0, 0, 0 } {}
Vec3::Vec3(const double x, const double y, const double z) : m_v{ x, y, z } {}

[[nodiscard]] double Vec3::x() const { return m_v[0]; }
[[nodiscard]] double Vec3::y() const { return m_v[1]; }
[[nodiscard]] double Vec3::z() const { return m_v[2]; }

double Vec3::operator[](const size_t index) const
{
  return m_v[index];// NOLINT(*-pro-bounds-constant-array-index)
}
double& Vec3::operator[](const size_t index)
{
  return m_v[index];// NOLINT(*-pro-bounds-constant-array-index)
}

Vec3 Vec3::operator-() const { return { -m_v[0], -m_v[1], -m_v[2] }; }
Vec3& Vec3::operator+=(const Vec3& other)
{
  m_v[0] += other.m_v[0];
  m_v[1] += other.m_v[1];
  m_v[2] += other.m_v[2];
  return *this;
}
Vec3& Vec3::operator*=(const double val)
{
  m_v[0] *= val;
  m_v[1] *= val;
  m_v[2] *= val;
  return *this;
}
Vec3& Vec3::operator/=(const double val) { return *this *= 1 / val; }
Vec3 Vec3::operator+(const Vec3& other) const
{
  return { m_v[0] + other.m_v[0], m_v[1] + other.m_v[1], m_v[2] + other.m_v[2] };
}
Vec3 Vec3::operator-(const Vec3& other) const
{
  return { m_v[0] - other.m_v[0], m_v[1] - other.m_v[1], m_v[2] - other.m_v[2] };
}
Vec3 Vec3::operator*(const Vec3& other) const
{
  return { m_v[0] * other.m_v[0], m_v[1] * other.m_v[1], m_v[2] * other.m_v[2] };
}
Vec3 operator*(const Vec3& vec, const double val) { return { vec.m_v[0] * val, vec.m_v[1] * val, vec.m_v[2] * val }; }
Vec3 operator*(const double val, const Vec3& vec) { return { vec.m_v[0] * val, vec.m_v[1] * val, vec.m_v[2] * val }; }
Vec3 Vec3::operator/(const double val) const { return { m_v[0] / val, m_v[1] / val, m_v[2] / val }; }

[[nodiscard]] double Vec3::length() const { return std::sqrt(length_squared()); }
[[nodiscard]] double Vec3::length_squared() const { return (m_v[0] * m_v[0]) + (m_v[1] * m_v[1]) + (m_v[2] * m_v[2]); }
[[nodiscard]] double Vec3::dot(const Vec3& other) const
{
  return (m_v[0] * other.m_v[0]) + (m_v[1] * other.m_v[1]) + (m_v[2] * other.m_v[2]);
}
[[nodiscard]] Vec3 Vec3::cross(const Vec3& other) const
{
  return { (m_v[1] * other.m_v[2]) - (m_v[2] * other.m_v[1]),
    (m_v[2] * other.m_v[0]) - (m_v[0] * other.m_v[2]),
    (m_v[0] * other.m_v[1]) - (m_v[1] * other.m_v[0]) };
}
[[nodiscard]] Vec3 Vec3::unit_vector() const { return *this / length(); }

constexpr auto std::formatter<Vec3>::parse(auto& ctx)
{
  auto it = ctx.begin();
  if (it == ctx.end()) { return it; }
  if (*it != '}') {
    std::println(std::cerr, "Invalid format args for Vec3");
    std::quick_exit(EXIT_FAILURE);
  }
  return it;
}

auto std::formatter<Vec3>::format(const Vec3& v, auto& ctx) const
{
  return std::format_to(ctx.out(), "({},{},{})", v.x(), v.y(), v.z());
}
