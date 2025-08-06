#ifndef SPHEREBVH_H
#define SPHEREBVH_H

#include "entities/Hittable.h"
#include "entities/Ray.h"
#include "entities/Sphere.h"
#include "entities/Vec3.h"

#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>
#include <vector>

struct SphereBVHNode
{
  Point3 aabb_min, aabb_max;
  std::uint32_t left_first{ 0U };// Index of either the first child node or the first sphere in this node
  std::uint32_t sphere_count{ 0U };// Number of speres in this node, or 0 if this is not a leaf node

  [[nodiscard]] bool is_leaf() const { return sphere_count > 0; }
};

class SphereBVH
{
public:
  explicit SphereBVH(std::span<const Sphere> spheres);
  [[nodiscard]] bool intersect(const Ray& r, double t_min, double t_max, HitRecord& rec) const;

private:
  void build_bvh();
  void subdivide(SphereBVHNode& node);
  // Gets the aabb_min and aabb_max of all spheres in the range [first_sphere_i, first_sphere_i + sphere_count)
  [[nodiscard]] std::pair<Point3, Point3> get_node_bounds(std::uint32_t first_sphere_i,
    std::uint32_t sphere_count) const;
  // Partitions the spheres in the range [l, r] (inclusive) based on the split value along the given axis
  // and returns the index of the first sphere in the right partition.
  std::uint32_t partition_spheres(std::uint32_t l, std::uint32_t r, int axis, double split_val);
  [[nodiscard]] static bool
    intersects_aabb(const SphereBVHNode& node, const Ray& r, const Vec3& r_dir_inv, double t_min, double t_max);

  // Finds the best split for the given node and returns the axis and split value.
  [[nodiscard]] static std::pair<int, double> find_best_split(const SphereBVHNode& node);

  std::span<const Sphere> m_spheres;
  std::vector<std::uint32_t> m_sphere_indices;
  std::vector<SphereBVHNode> m_nodes;
  thread_local static std::vector<std::uint32_t> m_traversal_stack;
};


#endif// SPHEREBVH_H
