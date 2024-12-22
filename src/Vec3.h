#ifndef VEC3_H
#define VEC3_H

#include <array>
#include <cstddef>
#include <cstdlib>
#include <format>
#include <iostream>
#include <ostream>

class Vec3;
// Point3 is just an alias for Vec3, but useful for geometric clarity in the code.
using Point3 = Vec3;

class Vec3
{
public:
  Vec3();
  Vec3(double x, double y, double z);

  [[nodiscard]] double x() const;
  [[nodiscard]] double y() const;
  [[nodiscard]] double z() const;

  double operator[](size_t index) const;
  double& operator[](size_t index);

  Vec3 operator-() const;

  Vec3& operator+=(const Vec3& other);
  Vec3& operator*=(double val);
  Vec3& operator/=(double val);

  Vec3 operator+(const Vec3& other) const;
  Vec3 operator-(const Vec3& other) const;
  Vec3 operator*(const Vec3& other) const;
  friend Vec3 operator*(const Vec3& vec, double val);
  friend Vec3 operator*(double val, const Vec3& vec);
  Vec3 operator/(double val) const;

  [[nodiscard]] double length() const;
  [[nodiscard]] double length_squared() const;
  [[nodiscard]] double dot(const Vec3& other) const;
  [[nodiscard]] Vec3 cross(const Vec3& other) const;
  [[nodiscard]] Vec3 unit_vector() const;

private:
  std::array<double, 3> m_v;
};

template<> struct std::formatter<Vec3>
{
  constexpr auto parse(auto& ctx)
  {
    auto it = ctx.begin();
    if (it == ctx.end() || *it == '}') { return it; }
    // Invalid format args for Vec3
    // TODO: Check performance of std::format_error with exceptions enabled
    std::quick_exit(EXIT_FAILURE);
  }
  auto format(const Vec3& v, auto& ctx) const { return std::format_to(ctx.out(), "({},{},{})", v.x(), v.y(), v.z()); }
};

#endif// VEC3_H
