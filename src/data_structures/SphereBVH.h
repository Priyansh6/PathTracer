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
  void subdivide(SphereBVHNode& node, int& nodes_used);
  // Sets the aabb_min and aabb_max of the node based on the spheres it contains
  // Note: Requires that left_first and sphere_count are set correctly
  void update_node_bounds(SphereBVHNode& node) const;
  // Partitions the spheres in the range [l, r] (inclusive) based on the split value along the given axis
  // and returns the index of the first sphere in the right partition.
  std::uint32_t partition_spheres(std::uint32_t l, std::uint32_t r, int axis, double split_val);
  [[nodiscard]] static bool intersects_aabb(const SphereBVHNode& node, const Ray& r, double t_min, double t_max);

  // Finds the best split for the given node and returns the axis and split value.
  [[nodiscard]] static std::pair<int, double> find_best_split(const SphereBVHNode& node);

  std::span<const Sphere> m_spheres;
  std::vector<std::uint32_t> m_sphere_indices;
  std::vector<SphereBVHNode> m_nodes;
};


#endif// SPHEREBVH_H
