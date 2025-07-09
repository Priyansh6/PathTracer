#include "Ray.h"
#include "Vec3.h"

Ray::Ray(const Point3& origin, const Vec3& direction) : m_origin{ origin }, m_direction{ direction } {}

[[nodiscard]] const Point3& Ray::origin() const { return m_origin; }
[[nodiscard]] const Vec3& Ray::direction() const { return m_direction; }

[[nodiscard]] Point3 Ray::at(const double t) const { return m_origin + t * m_direction; }