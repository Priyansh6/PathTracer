#include "Camera.h"
#include "Colour.h"
#include "Config.h"
#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"
#include "World.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <ostream>
#include <print>

namespace {
Colour ray_colour(const Ray& r, const World& world)
{
  if (HitRecord rec{}; world.hit(r, 0, std::numeric_limits<double>::infinity(), rec)) {
    return 0.5 * (rec.normal + Colour(1, 1, 1));
  }
  const Vec3 unit_direction = r.direction().unit_vector();
  const double a = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
}
}// namespace

int main()
{
  // Initialise the world.
  const World world(default_world::spheres);

  // Render the image.
  std::println("P3\n{} {}\n{}", config::image_width, config::image_height, colour::maximum_value);

  for (int i = 0; i < config::image_height; i++) {
    std::print(std::clog, "\rScanlines remaining: {:0>5}", config::image_height - i);
    std::clog.flush();
    for (int j = 0; j < config::image_width; j++) {
      const Point3 pixel_center = camera::pixel00_loc + (j * camera::pixel_delta_u) + (i * camera::pixel_delta_v);
      const Vec3 ray_direction = pixel_center - camera::camera_centre;
      const Ray r(camera::camera_centre, ray_direction);

      const Colour pixel_colour = ray_colour(r, world);
      write_colour(std::cout, pixel_colour);
    }
  }

  std::println(std::clog, "\rDone.                     ");
  return EXIT_SUCCESS;
}
