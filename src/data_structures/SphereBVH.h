#ifndef SPHEREBVH_H
#define SPHEREBVH_H

#include "entities/Hittable.h"
#include "entities/Ray.h"
#include "entities/Sphere.h"
#include "entities/Vec3.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <span>
#include <vector>

namespace sphere_bvh {
struct AABB
{
  Point3 bmin{ std::numeric_limits<double>::max() };
  Point3 bmax{ std::numeric_limits<double>::lowest() };

  void grow_to_fit_sphere(const Sphere& sphere)
  {
    bmin.x() = std::min(bmin.x(), sphere.centre().x() - sphere.radius());
    bmin.y() = std::min(bmin.y(), sphere.centre().y() - sphere.radius());
    bmin.z() = std::min(bmin.z(), sphere.centre().z() - sphere.radius());
    bmax.x() = std::max(bmax.x(), sphere.centre().x() + sphere.radius());
    bmax.y() = std::max(bmax.y(), sphere.centre().y() + sphere.radius());
    bmax.z() = std::max(bmax.z(), sphere.centre().z() + sphere.radius());
  }

  [[nodiscard]] double half_surface_area() const
  {
    const Vec3 extent = bmax - bmin;
    return (extent.x() * extent.y()) + (extent.x() * extent.z()) + (extent.y() * extent.z());
  }
};

struct SplitResult
{
  int axis;
  double split_value;// Anything lower non-inclusive should be on the left
  double cost;

  SplitResult(const int axis, const double split_value, const double cost)
    : axis{ axis }, split_value{ split_value }, cost{ cost } {};
};

struct SphereBVHNode
{
  AABB aabb;
  std::uint32_t left_first{ 0U };// Index of either the first child node or the first sphere in this node
  std::uint32_t sphere_count{ 0U };// Number of speres in this node, or 0 if this is not a leaf node

  [[nodiscard]] bool is_leaf() const { return sphere_count > 0; }
};
}// namespace sphere_bvh

class SphereBVH
{
public:
  explicit SphereBVH(std::span<const Sphere> spheres);
  [[nodiscard]] bool intersect(const Ray& r, double t_min, double t_max, HitRecord& rec) const;

private:
  void build_bvh();
  void subdivide(sphere_bvh::SphereBVHNode& node);
  // Gets the bmin and bmax of all spheres in the range [first_sphere_i, first_sphere_i + sphere_count)
  [[nodiscard]] sphere_bvh::AABB get_node_bounds(std::uint32_t first_sphere_i, std::uint32_t sphere_count) const;
  // Partitions the spheres in the range [l, r] (inclusive) based on the split value along the given axis
  // and returns the index of the first sphere in the right partition.
  std::uint32_t partition_spheres(std::uint32_t l, std::uint32_t r, int axis, double split_val);
  [[nodiscard]] static double intersects_aabb(const sphere_bvh::SphereBVHNode& node,
    const Ray& r,
    const Vec3& r_dir_reciprocal,
    double t_min,
    double t_max);
  [[nodiscard]] sphere_bvh::SplitResult find_best_split(const sphere_bvh::SphereBVHNode& node) const;
  [[nodiscard]] double
    evaluate_surface_area_heuristic(const sphere_bvh::SphereBVHNode& node, int axis, double split_pos) const;

  std::span<const Sphere> m_spheres;
  std::vector<std::uint32_t> m_sphere_indices;
  std::vector<sphere_bvh::SphereBVHNode> m_nodes;
  thread_local static std::vector<std::uint32_t> m_traversal_stack;
};


#endif// SPHEREBVH_H
