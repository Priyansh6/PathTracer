#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "Utils.h"
#include "Vec3.h"
#include "materials/Material.h"
#include "renderer/Colour.h"

#include <concepts>

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

template<class T>
concept Hittable = requires(const T& t, const Ray& r, const double t_min, const double t_max, HitRecord& rec) {
  { t.hit(r, t_min, t_max, rec) } -> std::same_as<bool>;
};


template<Hittable T>
bool hit(utils::input_range_of<T> auto&& hittable_objects,
  const Ray& r,
  const double t_min,
  const double t_max,
  HitRecord& rec)
{
  HitRecord temp_rec{};
  auto closest_so_far = t_max;

  for (const T& hittable_object : hittable_objects) {
    if (hittable_object.hit(r, t_min, closest_so_far, temp_rec)) {
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return closest_so_far < t_max;
}

#endif// HITTABLE_H
