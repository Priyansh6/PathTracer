#include "Scatter.h"
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
