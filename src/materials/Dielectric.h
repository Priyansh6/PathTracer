#ifndef DIELECTRIC_H
#define DIELECTRIC_H

class Dielectric
{
public:
  constexpr explicit Dielectric(const double refraction_index) : m_refraction_index(refraction_index) {}

  [[nodiscard]] constexpr double get_refraction_index() const { return m_refraction_index; }

private:
  // Refractive index in vacuum or air, or the ratio of the material's refractive index over
  // the refractive index of the enclosing media
  double m_refraction_index;
};

#endif// DIELECTRIC_H
