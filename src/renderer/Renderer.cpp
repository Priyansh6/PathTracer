#include "Renderer.h"
#include "Camera.h"
#include "Colour.h"
#include "Config.h"
#include "PpmWriter.h"
#include "Tile.h"
#include "WindowController.h"
#include "entities/Hittable.h"
#include "entities/Ray.h"
#include "entities/Vec3.h"
#include "entities/World.h"
#include "materials/Scatter.h"

#include <SDL3/SDL_timer.h>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <limits>
#include <oneapi/tbb/concurrent_queue.h>
#include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb/task_group.h>
#include <ostream>
#include <variant>
#include <vector>

constexpr double shadow_acne_epsilon = 0.0001;// Epsilon to prevent shadow acne.

Renderer::Renderer(const int image_width, const int image_height, const Camera& camera)
  : m_image_width{ image_width }, m_image_height{ image_height }, m_camera{ camera }
{}

void Renderer::render_to_ppm(const World& world,
  const int samples_per_pixel,
  const int max_depth,
  PpmWriter& ppm_writer) const
{
  std::vector buffer{ static_cast<std::size_t>(m_image_width * m_image_height), rgba::black_rgba };

  const auto start_time = std::chrono::steady_clock::now();

  tbb::parallel_for(0, m_image_height, [&](const int y) {
    for (int x = 0; x < m_image_width; x++) {
      const Colour pixel_colour = sample_pixel(x, y, world, samples_per_pixel, max_depth);
      buffer[(y * m_image_width) + x] = RGBA{ pixel_colour };
    }
  });

  const auto end_time = std::chrono::steady_clock::now();
  const auto duration = end_time - start_time;
  std::println(std::clog,
    "Rendering completed in {} seconds and {} milliseconds.",
    std::chrono::duration_cast<std::chrono::seconds>(duration).count(),
    std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000);// NOLINT(*-magic-numbers)

  ppm_writer.write_buffer(buffer);
}

void Renderer::render_to_window(const World& world,
  const int samples_per_pixel,
  const int max_depth,
  const int tile_size,
  WindowController& window_controller) const
{
  if (!window_controller.init_window()) {
    std::println(std::cerr, "Failed to initialize window.");
    return;
  }
  std::vector<Tile> tiles;
  for (int y = 0; y < m_image_height; y += tile_size) {
    for (int x = 0; x < m_image_width; x += tile_size) { tiles.push_back({ .x = x, .y = y }); }
  }
  std::size_t remaining_tiles = tiles.size();

  std::vector buffer{ static_cast<std::size_t>(m_image_width * m_image_height), rgba::black_rgba };
  tbb::concurrent_queue<Tile> result_tile_queue;
  tbb::task_group tg;

  for (const Tile& tile : tiles) {
    tg.run([&, tile] {
      for (int y = tile.y; y < tile.y + tile_size && y < m_image_height; ++y) {
        for (int x = tile.x; x < tile.x + tile_size && x < m_image_width; ++x) {
          const Colour pixel_colour = sample_pixel(x, y, world, samples_per_pixel, max_depth);
          buffer[(y * m_image_width) + x] = RGBA{ pixel_colour };
        }
      }

      result_tile_queue.emplace(tile.x, tile.y);
    });
  }

  const auto start_time = std::chrono::steady_clock::now();

  // Event Loop
  while (remaining_tiles > 0) {
    if (window_controller.poll_events_until_quit()) { return; }
    bool was_updated = false;
    Tile tile{};
    while (result_tile_queue.try_pop(tile)) {
      remaining_tiles--;
      window_controller.update_tile(tile, tile_size, buffer);
      was_updated = true;
    }
    if (was_updated) { window_controller.present(); }
    SDL_Delay(window_config::min_frame_time_ms);// Prevent busy-waiting
  }

  const auto end_time = std::chrono::steady_clock::now();
  const auto duration = end_time - start_time;
  std::println(std::clog,
    "Rendering completed in {} seconds and {} milliseconds.",
    std::chrono::duration_cast<std::chrono::seconds>(duration).count(),
    std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000);// NOLINT(*-magic-numbers)
}

Colour Renderer::sample_pixel(const int x,
  const int y,
  const World& world,
  const int samples_per_pixel,
  const int max_depth) const
{
  Colour pixel_colour = { 0, 0, 0 };
  for (int sample = 0; sample < samples_per_pixel; sample++) {
    Ray const sample_ray = m_camera.get_ray(x, y);
    pixel_colour += trace_and_shade(sample_ray, world, max_depth);
  }
  pixel_colour /= samples_per_pixel;
  return pixel_colour;
}

Colour Renderer::trace_and_shade(const Ray& r, const World& world, const int max_depth)
{
  Colour curr_diffuse_reflection_coefficient = { 1.0, 1.0, 1.0 };
  Ray curr_ray = r;

  // Perform bounces
  for (int depth = 0; depth < max_depth; depth++) {
    HitRecord rec{};
    if (!world.hit(curr_ray, shadow_acne_epsilon, std::numeric_limits<double>::max(), rec)) {
      // Ray did not hit anything, return background colour.
      const Vec3 unit_direction = curr_ray.direction().unit_vector();
      const double a = (unit_direction.y() + 1.0) / 2;
      return curr_diffuse_reflection_coefficient
             * ((1.0 - a) * world.background_bottom_colour() + a * world.background_top_colour());
    }
    ScatterRecord s_rec{};
    if (!std::visit([&](const auto& material) { return Scatter{}(material, curr_ray, rec, s_rec); }, *rec.material)) {
      // Ray hit an object but did not scatter and was absorbed, return black.
      return colour::black;
    }
    // Ray scattered, accumulate colour.
    curr_diffuse_reflection_coefficient *= s_rec.attenuation;
    curr_ray = s_rec.scattered;
  }
  // Return black if max depth reached without being absorbed.
  return colour::black;
}