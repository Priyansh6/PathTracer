#include "Config.h"
#include "entities/World.h"
#include "renderer/Camera.h"

#include <cstdlib>

int main()
{
  const World world(
    default_world::background_top_colour, default_world::background_bottom_colour, default_world::generate_spheres());
  const Camera camera(config::aspect_ratio,
    config::image_width,
    config::samples_per_pixel,
    config::max_depth,
    config::vertical_fov,
    config::look_from,
    config::look_at,
    config::vup,
    config::defocus_angle,
    config::focus_dist);
  camera.render(world);
  return EXIT_SUCCESS;
}
