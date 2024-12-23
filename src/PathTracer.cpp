#include "Camera.h"
#include "Config.h"
#include "World.h"

#include <cstdlib>

int main()
{
  const World world(default_world::spheres);
  const Camera camera(config::aspect_ratio, config::image_width, config::samples_per_pixel);
  camera.render(world);
  return EXIT_SUCCESS;
}
