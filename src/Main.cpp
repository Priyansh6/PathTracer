#include "Config.h"
#include "entities/World.h"
#include "renderer/Camera.h"
#include "renderer/Colour.h"
#include "renderer/PpmPixelWriter.h"
#include "renderer/Renderer.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdlib>
#include <fstream>
#include <vector>

constexpr bool SDL_ENABLE = false;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  constexpr int image_height =
    static_cast<int>(static_cast<double>(window_config::image_width) / window_config::aspect_ratio);

  if (SDL_ENABLE) {
    SDL_SetAppMetadata(app_metadata::name.c_str(), app_metadata::version.c_str(), app_metadata::id.c_str());
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());// NOLINT(*-vararg)
      return SDL_APP_FAILURE;
    }
    SDL_Window* sdl_window = nullptr;
    SDL_Renderer* sdl_renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer(
          "path-tracer", window_config::image_width, image_height, 0, &sdl_window, &sdl_renderer)) {
      SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());// NOLINT(*-vararg)
      return SDL_APP_FAILURE;
    }

    const std::vector<Colour> pixels{ static_cast<std::size_t>(window_config::image_width)
                                      * static_cast<std::size_t>(image_height) };
  }

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
  std::ofstream output_file{ "output.ppm" };
  PpmPixelWriter pixel_writer{ window_config::image_width, image_height, output_file };
  const Renderer renderer{ window_config::image_width, image_height, camera, pixel_writer };
  renderer.render(world, rt_config::samples_per_pixel, rt_config::max_depth);
  output_file.close();
  return EXIT_SUCCESS;
}
