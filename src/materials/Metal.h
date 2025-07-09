#ifndef METAL_H
#define METAL_H

#include "renderer/Colour.h"

class Metal
{
public:
  constexpr explicit Metal(const Colour& albedo, const double fuzz) : m_albedo{ albedo }, m_fuzz{ fuzz } {}

  [[nodiscard]] constexpr Colour get_albedo() const { return m_albedo; }
  [[nodiscard]] constexpr double get_fuzz() const { return m_fuzz; }

private:
  Colour m_albedo;
  double m_fuzz;
};

#endif// METAL_H
