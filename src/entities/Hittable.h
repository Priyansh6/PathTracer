#ifndef HITTABLE_H
#define HITTABLE_H

#include "Vec3.h"
#include "materials/Material.h"

struct HitRecord
{
  Point3 point;
  Vec3 normal;
  double t{ 0 };
  bool front_face{ false };
  const Material* material{};
};

#endif// HITTABLE_H
