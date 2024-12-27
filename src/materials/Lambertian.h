#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "renderer/Colour.h"

class Lambertian
{
public:
  constexpr explicit Lambertian(const Colour& albedo) : m_albedo(albedo) {}

  [[nodiscard]] constexpr Colour get_albedo() const { return m_albedo; }

private:
  Colour m_albedo;
};

#endif// LAMBERTIAN_H
