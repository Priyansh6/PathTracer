#include "Config.h"
#include "entities/World.h"
#include "renderer/Camera.h"

#include <cstdlib>

int main()
{
  const World world(default_world::spheres);
  const Camera camera(config::aspect_ratio, config::image_width, config::samples_per_pixel, config::max_depth);
  camera.render(world);
  std::cout.flush();
  return EXIT_SUCCESS;
}
