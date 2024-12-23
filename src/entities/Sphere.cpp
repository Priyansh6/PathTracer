#include "Sphere.h"
#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"

#include <cmath>

bool Sphere::hit(const Ray& r, const double min_t, const double max_t, HitRecord& rec) const
{
  const Vec3 oc = m_centre - r.origin();
  const auto a = r.direction().length_squared();
  const auto h = r.direction().dot(oc);
  const auto c = oc.length_squared() - (m_radius * m_radius);

  const auto discriminant = (h * h) - (a * c);
  if (discriminant < 0) { return false; }

  const auto discriminant_sqrt = std::sqrt(discriminant);

  // Find the nearest root that lies (non-inclusive) in the acceptable range.
  auto root = (h - discriminant_sqrt) / a;
  if (root <= min_t || root >= max_t) {
    root = (h + discriminant_sqrt) / a;
    if (root <= min_t || root >= max_t) { return false; }
  }

  rec.t = root;
  rec.point = r.at(rec.t);

  const Vec3 outward_normal = (rec.point - m_centre) / m_radius;
  const bool front_face = r.direction().dot(outward_normal) < 0;
  rec.front_face = front_face;
  rec.normal = front_face ? outward_normal : -outward_normal;

  return true;
}