#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "Vec3.h"
#include "materials/Material.h"
#include "renderer/Colour.h"

struct HitRecord
{
  Point3 point;
  Vec3 normal;
  double t{ 0 };
  bool front_face{ false };
  const Material* material{};
};

struct ScatterRecord
{
  Ray scattered;
  Colour attenuation;
};

#endif// HITTABLE_H
