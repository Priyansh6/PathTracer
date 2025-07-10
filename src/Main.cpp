#include "Config.h"
#include "entities/World.h"
#include "renderer/Camera.h"
#include "renderer/PpmWriter.h"
#include "renderer/Renderer.h"
#include "renderer/WindowController.h"

#include <SDL3/SDL_main.h>
#include <cstdlib>
#include <fstream>

constexpr bool use_window_display = true;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  constexpr int image_height =
    static_cast<int>(static_cast<double>(window_config::image_width) / window_config::aspect_ratio);

  const World world{
    default_world::background_top_colour, default_world::background_bottom_colour, default_world::generate_spheres()
  };
  const Camera camera{ window_config::image_width,
    image_height,
    camera_config::vertical_fov,
    camera_config::look_from,
    camera_config::look_at,
    camera_config::vup,
    camera_config::defocus_angle,
    camera_config::focus_dist };

  if (use_window_display) {
    WindowController window_controller{ window_config::image_width, image_height };
    const Renderer renderer{ window_config::image_width, image_height, camera };
    renderer.render_to_window(world, rt_config::samples_per_pixel, rt_config::max_depth, window_controller);
  } else {
    std::ofstream output_file{ "output.ppm" };
    PpmWriter ppm_writer{ window_config::image_width, image_height, output_file };
    const Renderer renderer{ window_config::image_width, image_height, camera };
    renderer.render_to_ppm(world, rt_config::samples_per_pixel, rt_config::max_depth, ppm_writer);
    output_file.close();
  }
  return EXIT_SUCCESS;
}
