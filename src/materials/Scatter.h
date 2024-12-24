#ifndef SCATTER_H
#define SCATTER_H

#include "Lambertian.h"
#include "Metal.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"

struct Scatter
{
  bool operator()(const Lambertian& lambertian, const Ray& /*r_in*/, const HitRecord& rec, ScatterRecord& s_rec) const;
  bool operator()(const Metal& metal, const Ray& r_in, const HitRecord& rec, ScatterRecord& s_rec) const;
};

#endif// SCATTER_H
