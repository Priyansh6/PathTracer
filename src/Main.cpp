#include "Config.h"
#include "entities/World.h"
#include "renderer/Camera.h"
#include "renderer/PpmWriter.h"
#include "renderer/Renderer.h"
#include "renderer/WindowController.h"

#include <SDL3/SDL_main.h>
#include <cstdlib>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <oneapi/tbb/global_control.h>
#include <optional>
#include <print>
#include <string>

int main(int argc, char* argv[])
{
  // Parse command line arguments.
  cxxopts::Options options{ app_metadata::name, "A simple path tracer." };
  options.add_options()("m,output-mode",
    "The output mode for the renderer (window or ppm).",
    cxxopts::value<std::string>()->default_value("window"))("t,max-threads",
    "The maximum number of threads to use for rendering. (0 for auto-detect, > 0 for specific number).",
    cxxopts::value<int>()->default_value("0"))("o,ppm-output",
    "The file which to store the rendered image in PPM format (only enabled with ppm output mode).",
    cxxopts::value<std::string>()->default_value("output.ppm"))("w,width",
    "The width of the resulting image in pixels",
    cxxopts::value<int>()->default_value(std::to_string(window_config::default_width)))("b,max-bounces",
    "The maximum number of bounces per ray.",
    cxxopts::value<int>()->default_value(std::to_string(rt_config::default_max_depth)))("p,samples-per-pixel",
    "The number of ray samples per pixel.",
    cxxopts::value<int>()->default_value(std::to_string(rt_config::default_samples_per_pixel)))(
    "h,help", "Print usage information.");
  auto parse_result = options.parse(argc, argv);

  if (parse_result.contains("help")) {
    std::println("{}", options.help());
    return EXIT_SUCCESS;
  }

  // Set the maximum number of threads for TBB if specified.
  const int max_threads = parse_result["max-threads"].as<int>();
  if (max_threads < 0) {
    std::println(std::cerr, "Invalid number of max threads: {}. Must be a non-negative integer.", max_threads);
    return EXIT_FAILURE;
  }
  std::optional<tbb::global_control> gc;
  if (max_threads != 0) { gc.emplace(tbb::global_control::max_allowed_parallelism, max_threads + 1); }

  // Validate and set the image width and height.
  const int image_width = parse_result["width"].as<int>();
  if (image_width <= 0) {
    std::println(std::cerr, "Invalid image width: {}. Must be a positive integer.", image_width);
    return EXIT_FAILURE;
  }
  const int image_height = static_cast<int>(static_cast<double>(image_width) / window_config::aspect_ratio);

  // Validate and set the samples per pixel.
  const int samples_per_pixel = parse_result["samples-per-pixel"].as<int>();
  if (samples_per_pixel <= 0) {
    std::println(std::cerr, "Invalid samples per pixel: {}. Must be a positive integer.", samples_per_pixel);
    return EXIT_FAILURE;
  }
  // Validate and set the maximum bounces.
  const int max_bounces = parse_result["max-bounces"].as<int>();
  if (max_bounces <= 0) {
    std::println(std::cerr, "Invalid max bounces: {}. Must be a positive integer.", max_bounces);
    return EXIT_FAILURE;
  }

  // Create the world and camera.
  const World world{
    default_world::background_top_colour, default_world::background_bottom_colour, default_world::generate_spheres()
  };
  const Camera camera{ image_width,
    image_height,
    camera_config::vertical_fov,
    camera_config::look_from,
    camera_config::look_at,
    camera_config::vup,
    camera_config::defocus_angle,
    camera_config::focus_dist };

  // Determine the output mode and render the scene accordingly.
  const std::string output_mode = parse_result["output-mode"].as<std::string>();

  if (output_mode == "window") {
    WindowController window_controller{ image_width, image_height };
    const Renderer renderer{ image_width, image_height, camera };
    renderer.render_to_window(world, samples_per_pixel, max_bounces, window_controller);
    window_controller.wait_for_quit();
  } else if (output_mode == "ppm") {
    std::ofstream output_file{ parse_result["ppm-output"].as<std::string>() };
    PpmWriter ppm_writer{ image_width, image_height, output_file };
    const Renderer renderer{ image_width, image_height, camera };
    renderer.render_to_ppm(world, samples_per_pixel, max_bounces, ppm_writer);
    output_file.close();
  } else {
    std::println(std::cerr, "Invalid output mode: '{}'. Expected 'window' or 'ppm'.", output_mode);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
