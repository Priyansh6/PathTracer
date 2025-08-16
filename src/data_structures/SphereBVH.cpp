#include "SphereBVH.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"
#include "entities/Sphere.h"
#include "entities/Vec3.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <print>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

using namespace sphere_bvh;

SphereBVH::SphereBVH(const std::span<const Sphere> spheres)
  : m_spheres{ spheres }, m_sphere_indices(std::views::iota(0U, spheres.size()) | std::ranges::to<std::vector>())
{
  m_nodes.reserve((2 * m_spheres.size()) - 1);
  std::println("{}", m_nodes.size());
  build_bvh();
  std::println("{}", m_nodes.size());
}

thread_local std::vector<std::uint32_t> SphereBVH::m_traversal_stack;

bool SphereBVH::intersect(const Ray& r, const double t_min, const double t_max, HitRecord& rec) const
{
  auto closest_t = t_max;
  const Vec3 r_dir_reciprocal = 1.0 / r.direction();

  m_traversal_stack.clear();
  m_traversal_stack.reserve(m_nodes.size());
  m_traversal_stack.push_back(0U);

  while (!m_traversal_stack.empty()) {
    const std::uint32_t node_i = m_traversal_stack.back();
    m_traversal_stack.pop_back();
    const SphereBVHNode& node = m_nodes[node_i];

    if (!intersects_aabb(node, r, r_dir_reciprocal, t_min, t_max)) { continue; }
    if (node.is_leaf()) {
      for (std::uint32_t i = node.left_first; i < (node.left_first + node.sphere_count); i++) {
        HitRecord temp_rec{};
        if (const Sphere& sphere = m_spheres[m_sphere_indices[i]]; sphere.hit(r, t_min, closest_t, temp_rec)) {
          closest_t = temp_rec.t;
          rec = temp_rec;
        }
      }
    } else {
      m_traversal_stack.push_back(node.left_first);
      m_traversal_stack.push_back(node.left_first + 1);
    }
  }

  return closest_t < t_max;
}

void SphereBVH::build_bvh()
{
  constexpr std::uint32_t root_node_i = 0U;
  constexpr std::uint32_t first_sphere_i = 0U;
  const AABB aabb = get_node_bounds(first_sphere_i, m_spheres.size());
  m_nodes.emplace_back(aabb, first_sphere_i, m_spheres.size());
  subdivide(m_nodes[root_node_i]);
}

void SphereBVH::subdivide(SphereBVHNode& node)
{
  Vec3 extent = node.aabb.bmax - node.aabb.bmin;
  const double half_parent_surface_area =
    (extent.x() * extent.y()) + (extent.x() * extent.z()) + (extent.y() * extent.z());
  const double parent_cost = half_parent_surface_area * node.sphere_count;

  if (node.sphere_count <= 1) { return; }
  auto [axis, split_pos, cost] = find_best_split(node);
  if (cost >= parent_cost) { return; }

  const std::uint32_t l = node.left_first;
  const std::uint32_t r = node.left_first + node.sphere_count - 1;
  const std::uint32_t split_i = partition_spheres(l, r, axis, split_pos);

  const std::uint32_t left_count = split_i - l;
  if (left_count == 0 || left_count == node.sphere_count) { return; }

  const std::uint32_t left_i = m_nodes.size();

  const std::uint32_t left_left_first = node.left_first;
  const std::uint32_t left_sphere_count = left_count;
  const std::uint32_t right_left_first = split_i;
  const std::uint32_t right_sphere_count = node.sphere_count - left_count;

  const AABB left_aabb = get_node_bounds(left_left_first, left_sphere_count);
  const AABB right_aabb = get_node_bounds(right_left_first, right_sphere_count);

  m_nodes.emplace_back(left_aabb, left_left_first, left_sphere_count);
  m_nodes.emplace_back(right_aabb, right_left_first, right_sphere_count);

  node.sphere_count = 0U;
  node.left_first = left_i;

  subdivide(m_nodes[left_i]);
  subdivide(m_nodes[left_i + 1]);
}

AABB SphereBVH::get_node_bounds(const std::uint32_t first_sphere_i, const std::uint32_t sphere_count) const
{
  AABB aabb{};
  for (std::uint32_t i = first_sphere_i; i < (first_sphere_i + sphere_count); i++) {
    const Sphere& sphere = m_spheres[m_sphere_indices[i]];
    aabb.grow_to_fit_sphere(sphere);
  }
  return aabb;
}

std::uint32_t SphereBVH::partition_spheres(std::uint32_t l, std::uint32_t r, const int axis, const double split_val)
{
  while (l <= r) {
    if (const Sphere& sphere = m_spheres[m_sphere_indices[l]]; sphere.centre()[axis] < split_val) {
      l++;
    } else {
      std::swap(m_sphere_indices[l], m_sphere_indices[r]);
      r--;
    }
  }

  return l;
}

bool SphereBVH::intersects_aabb(const SphereBVHNode& node,
  const Ray& r,
  const Vec3& r_dir_reciprocal,
  double t_min,
  double t_max)
{
  const double tx1 = (node.aabb.bmin.x() - r.origin().x()) * r_dir_reciprocal.x();
  const double tx2 = (node.aabb.bmax.x() - r.origin().x()) * r_dir_reciprocal.x();

  t_min = std::max(t_min, std::min(tx1, tx2));
  t_max = std::min(t_max, std::max(tx1, tx2));

  const double ty1 = (node.aabb.bmin.y() - r.origin().y()) * r_dir_reciprocal.y();
  const double ty2 = (node.aabb.bmax.y() - r.origin().y()) * r_dir_reciprocal.y();
  t_min = std::max(t_min, std::min(ty1, ty2));
  t_max = std::min(t_max, std::max(ty1, ty2));

  const double tz1 = (node.aabb.bmin.z() - r.origin().z()) * r_dir_reciprocal.z();
  const double tz2 = (node.aabb.bmax.z() - r.origin().z()) * r_dir_reciprocal.z();
  t_min = std::max(t_min, std::min(tz1, tz2));
  t_max = std::min(t_max, std::max(tz1, tz2));

  return t_max >= t_min;
}

SplitResult SphereBVH::find_best_split(const SphereBVHNode& node) const
{
  SplitResult best_split{ 0, 0.0, std::numeric_limits<double>::max() };
  for (int axis = 0; axis < 3; axis++) {
    for (uint32_t i = 0U; i < node.sphere_count; i++) {
      const Sphere& sphere = m_spheres[m_sphere_indices[node.left_first + i]];
      const double split_pos = sphere.centre()[axis];
      if (const double cost = evaluate_surface_area_heuristic(node, axis, split_pos); cost < best_split.cost) {
        best_split.cost = cost;
        best_split.axis = axis;
        best_split.split_value = split_pos;
      }
    }
  }
  return best_split;
}

double
  SphereBVH::evaluate_surface_area_heuristic(const SphereBVHNode& node, const int axis, const double split_pos) const
{
  AABB left_box{};
  AABB right_box{};
  int left_count = 0;
  int right_count = 0;
  for (std::uint32_t i = 0U; i < node.sphere_count; i++) {
    if (const Sphere& sphere = m_spheres[m_sphere_indices[node.left_first + i]]; sphere.centre()[axis] < split_pos) {
      left_count++;
      left_box.grow_to_fit_sphere(sphere);
    } else {
      right_count++;
      right_box.grow_to_fit_sphere(sphere);
    }
  }
  return (left_count * left_box.half_surface_area()) + (right_count * right_box.half_surface_area());
}