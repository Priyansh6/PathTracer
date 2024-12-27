#include "Scatter.h"
#include "Dielectric.h"
#include "Lambertian.h"
#include "Metal.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"

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
  const Vec3 refracted = r_in.direction().unit_vector().refract(rec.normal, ri);

  s_rec.attenuation = Colour(1.0, 1.0, 1.0);
  s_rec.scattered = Ray(rec.point, refracted);
  return true;
}
