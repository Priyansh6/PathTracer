#include "SphereBVH.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"
#include "entities/Sphere.h"
#include "entities/Vec3.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <queue>
#include <ranges>
#include <span>
#include <stack>
#include <utility>
#include <vector>

SphereBVH::SphereBVH(const std::span<const Sphere> spheres)
  : m_spheres{ spheres }, m_sphere_indices(std::views::iota(0U, spheres.size()) | std::ranges::to<std::vector>())
{
  m_nodes.reserve((2 * m_spheres.size()) - 1);
  build_bvh();
}

bool SphereBVH::intersect(const Ray& r, const double t_min, const double t_max, HitRecord& rec) const
{
  auto closest_t = t_max;

  std::stack<std::uint32_t> node_i_queue{ { 0U } };
  while (!node_i_queue.empty()) {
    const std::uint32_t node_i = node_i_queue.top();
    node_i_queue.pop();
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
      node_i_queue.push(node.left_first);
      node_i_queue.push(node.left_first + 1);
    }
  }

  return closest_t < t_max;
}

void SphereBVH::build_bvh()
{
  SphereBVHNode& root_node = m_nodes[0];
  root_node.left_first = 0;
  root_node.sphere_count = m_spheres.size();
  update_node_bounds(root_node);

  int nodes_used = 1;
  subdivide(root_node, nodes_used);
}

void SphereBVH::subdivide(SphereBVHNode& node, int& nodes_used)
{
  if (node.sphere_count <= 1) { return; }

  auto [axis, splitPos] = find_best_split(node);

  const std::uint32_t l = node.left_first;
  const std::uint32_t r = node.left_first + node.sphere_count - 1;
  const std::uint32_t split_i = partition_spheres(l, r, axis, splitPos);

  const std::uint32_t left_count = split_i - l;
  if (left_count == 0 || left_count == node.sphere_count) { return; }

  const int left_i = nodes_used++;
  const int right_i = nodes_used++;

  SphereBVHNode& left_child = m_nodes[left_i];
  SphereBVHNode& right_child = m_nodes[right_i];
  left_child.left_first = node.left_first;
  left_child.sphere_count = left_count;
  right_child.left_first = split_i;
  right_child.sphere_count = node.sphere_count - left_count;
  node.sphere_count = 0;
  node.left_first = left_i;

  update_node_bounds(left_child);
  update_node_bounds(right_child);

  subdivide(left_child, nodes_used);
  subdivide(right_child, nodes_used);
}

void SphereBVH::update_node_bounds(SphereBVHNode& node) const
{
  node.aabb_min = Point3{ std::numeric_limits<double>::max() };
  node.aabb_max = Point3{ std::numeric_limits<double>::lowest() };
  for (std::uint32_t i = node.left_first; i < (node.left_first + node.sphere_count); i++) {
    const Sphere& sphere = m_spheres[m_sphere_indices[i]];
    node.aabb_min.x() = std::min(node.aabb_min.x(), sphere.centre().x() - sphere.radius());
    node.aabb_min.y() = std::min(node.aabb_min.y(), sphere.centre().y() - sphere.radius());
    node.aabb_min.z() = std::min(node.aabb_min.z(), sphere.centre().z() - sphere.radius());
    node.aabb_max.x() = std::max(node.aabb_max.x(), sphere.centre().x() + sphere.radius());
    node.aabb_max.y() = std::max(node.aabb_max.y(), sphere.centre().y() + sphere.radius());
    node.aabb_max.z() = std::max(node.aabb_max.z(), sphere.centre().z() + sphere.radius());
  }
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