#include "SphereBVH.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"
#include "entities/Sphere.h"
#include "entities/Vec3.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

SphereBVH::SphereBVH(const std::span<const Sphere> spheres)
  : m_spheres{ spheres }, m_sphere_indices(std::views::iota(0U, spheres.size()) | std::ranges::to<std::vector>())
{
  m_nodes.reserve((2 * m_spheres.size()) - 1);
  build_bvh();
}

thread_local std::vector<std::uint32_t> SphereBVH::m_traversal_stack;

bool SphereBVH::intersect(const Ray& r, const double t_min, const double t_max, HitRecord& rec) const
{
  auto closest_t = t_max;

  m_traversal_stack.clear();
  m_traversal_stack.reserve(m_nodes.size());
  m_traversal_stack.push_back(0U);

  while (!m_traversal_stack.empty()) {
    const std::uint32_t node_i = m_traversal_stack.back();
    m_traversal_stack.pop_back();
    const SphereBVHNode& node = m_nodes[node_i];

    if (!intersects_aabb(node, r, t_min, t_max)) { continue; }
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
  auto [aabb_min, aabb_max] = get_node_bounds(root_node_i, m_spheres.size());
  m_nodes.emplace_back(aabb_min, aabb_max, 0U, m_spheres.size());
  subdivide(m_nodes[root_node_i]);
}

void SphereBVH::subdivide(SphereBVHNode& node)
{
  if (node.sphere_count <= 1) { return; }

  auto [axis, splitPos] = find_best_split(node);

  const std::uint32_t l = node.left_first;
  const std::uint32_t r = node.left_first + node.sphere_count - 1;
  const std::uint32_t split_i = partition_spheres(l, r, axis, splitPos);

  const std::uint32_t left_count = split_i - l;
  if (left_count == 0 || left_count == node.sphere_count) { return; }

  const std::uint32_t left_i = m_nodes.size();

  const std::uint32_t left_left_first = node.left_first;
  const std::uint32_t left_sphere_count = left_count;
  const std::uint32_t right_left_first = split_i;
  const std::uint32_t right_sphere_count = node.sphere_count - left_count;

  auto [left_aabb_min, left_aabb_max] = get_node_bounds(left_left_first, left_sphere_count);
  auto [right_aabb_min, right_aabb_max] = get_node_bounds(right_left_first, right_sphere_count);

  m_nodes.emplace_back(left_aabb_min, left_aabb_max, left_left_first, left_sphere_count);
  m_nodes.emplace_back(right_aabb_min, right_aabb_max, right_left_first, right_sphere_count);

  node.sphere_count = 0U;
  node.left_first = left_i;

  subdivide(m_nodes[left_i]);
  subdivide(m_nodes[left_i + 1]);
}

std::pair<Point3, Point3> SphereBVH::get_node_bounds(const std::uint32_t first_sphere_i,
  const std::uint32_t sphere_count) const
{
  std::pair<Point3, Point3> bounds;
  auto& [aabb_min, aabb_max] = bounds;
  aabb_min = Point3{ std::numeric_limits<double>::max() };
  aabb_max = Point3{ std::numeric_limits<double>::lowest() };
  for (std::uint32_t i = first_sphere_i; i < (first_sphere_i + sphere_count); i++) {
    const Sphere& sphere = m_spheres[m_sphere_indices[i]];
    aabb_min.x() = std::min(aabb_min.x(), sphere.centre().x() - sphere.radius());
    aabb_min.y() = std::min(aabb_min.y(), sphere.centre().y() - sphere.radius());
    aabb_min.z() = std::min(aabb_min.z(), sphere.centre().z() - sphere.radius());
    aabb_max.x() = std::max(aabb_max.x(), sphere.centre().x() + sphere.radius());
    aabb_max.y() = std::max(aabb_max.y(), sphere.centre().y() + sphere.radius());
    aabb_max.z() = std::max(aabb_max.z(), sphere.centre().z() + sphere.radius());
  }
  return bounds;
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

bool SphereBVH::intersects_aabb(const SphereBVHNode& node, const Ray& r, double t_min, double t_max)
{
  constexpr double epsilon = 1e-50;

  if (std::abs(r.direction().x()) < epsilon
      && (r.origin().x() < node.aabb_min.x() || r.origin().x() > node.aabb_max.x())) {
    return {};
  }
  const double tx1 = (node.aabb_min.x() - r.origin().x()) / r.direction().x();
  const double tx2 = (node.aabb_max.x() - r.origin().x()) / r.direction().x();
  t_min = std::max(t_min, std::min(tx1, tx2));
  t_max = std::min(t_max, std::max(tx1, tx2));

  if (std::abs(r.direction().y()) < epsilon
      && (r.origin().y() < node.aabb_min.y() || r.origin().y() > node.aabb_max.y())) {
    return {};
  }
  const double ty1 = (node.aabb_min.y() - r.origin().y()) / r.direction().y();
  const double ty2 = (node.aabb_max.y() - r.origin().y()) / r.direction().y();
  t_min = std::max(t_min, std::min(ty1, ty2));
  t_max = std::min(t_max, std::max(ty1, ty2));

  if (std::abs(r.direction().z()) < epsilon
      && (r.origin().z() < node.aabb_min.z() || r.origin().z() > node.aabb_max.z())) {
    return {};
  }
  const double tz1 = (node.aabb_min.z() - r.origin().z()) / r.direction().z();
  const double tz2 = (node.aabb_max.z() - r.origin().z()) / r.direction().z();
  t_min = std::max(t_min, std::min(tz1, tz2));
  t_max = std::min(t_max, std::max(tz1, tz2));

  // If the final interval is valid, we have a hit within the desired range.
  return t_max >= t_min;
}

std::pair<int, double> SphereBVH::find_best_split(const SphereBVHNode& node)
{
  Vec3 extent = node.aabb_max - node.aabb_min;
  int axis = 0;
  if (extent.y() > extent.x()) { axis = 1; }
  if (extent.z() > extent[axis]) { axis = 2; }
  const double splitPos = (node.aabb_min[axis] + node.aabb_max[axis]) / 2.0;
  return { axis, splitPos };
}