#include "Scatter.h"
#include "Dielectric.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Utils.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"
#include <algorithm>
#include <cmath>

namespace {
double reflectance(const double cosine, const double refraction_index)
{
  // Use Schlick's approximation for reflectance.
  auto r0 = (1 - refraction_index) / (1 + refraction_index);
  r0 = r0 * r0;
  return r0 + ((1 - r0) * std::pow(1 - cosine, 5));// NOLINT(*-magic-numbers)
}
}// namespace

bool Scatter::operator()(const Lambertian& lambertian,
  const Ray& /*r_in*/,
  const HitRecord& rec,
  ScatterRecord& s_rec) const
{
  auto scatter_direction = rec.normal + Vec3::random_unit_vector();

  // Catch degenerate scatter direction
  if (scatter_direction.near_zero()) { scatter_direction = rec.normal; }

  s_rec.scattered = Ray(rec.point, scatter_direction);
  s_rec.attenuation = lambertian.get_albedo();
  return true;
}

bool Scatter::operator()(const Metal& metal, const Ray& r_in, const HitRecord& rec, ScatterRecord& s_rec) const
{
  Vec3 reflected = r_in.direction().reflect(rec.normal);
  reflected = reflected.unit_vector() + (metal.get_fuzz() * Vec3::random_unit_vector());
  s_rec.scattered = Ray(rec.point, reflected);
  s_rec.attenuation = metal.get_albedo();
  return reflected.dot(rec.normal) > 0;
}

bool Scatter::operator()(const Dielectric& dielectric,
  const Ray& r_in,
  const HitRecord& rec,
  ScatterRecord& s_rec) const
{
  const double ri = rec.front_face ? (1.0 / dielectric.get_refraction_index()) : dielectric.get_refraction_index();
  const Vec3 unit_dir = r_in.direction().unit_vector();

  const double cos_theta = std::min(-unit_dir.dot(rec.normal), 1.0);
  const double sin_theta = std::sqrt(1.0 - (cos_theta * cos_theta));

  const bool cannot_refract = ri * sin_theta > 1.0;
  const Vec3 direction = cannot_refract || reflectance(cos_theta, ri) > utils::random_double()
                           ? unit_dir.reflect(rec.normal)
                           : unit_dir.refract(rec.normal, ri);

  s_rec.attenuation = Colour(1.0, 1.0, 1.0);
  s_rec.scattered = Ray(rec.point, direction);
  return true;
}
