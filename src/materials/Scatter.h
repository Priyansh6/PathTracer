#ifndef SCATTER_H
#define SCATTER_H

#include "Dielectric.h"
#include "Lambertian.h"
#include "Metal.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"

// ScatterRecord holds the result of scattering a ray off a material.
// It contains the scattered ray and the attenuation colour.
struct ScatterRecord
{
  Ray scattered;
  Colour attenuation;
};

// Scatter is a visitor struct that defines how to scatter rays based on the material type.
// It uses the visitor pattern to handle different material types: Lambertian, Metal, and Dielectric.
struct Scatter
{
  bool operator()(const Lambertian& lambertian, const Ray& /*r_in*/, const HitRecord& rec, ScatterRecord& s_rec) const;
  bool operator()(const Metal& metal, const Ray& r_in, const HitRecord& rec, ScatterRecord& s_rec) const;
  bool operator()(const Dielectric& dielectric, const Ray& r_in, const HitRecord& rec, ScatterRecord& s_rec) const;
};

#endif// SCATTER_H
