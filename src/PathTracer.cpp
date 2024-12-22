#include <cstdlib>
#include <print>

#include "Ray.h"
#include "Vec3.h"

int main()
{
  const Vec3 v1(3, 2, 1);
  const Vec3 v2(3, 3, 6);

  const Ray r(v1, v2);
  std::println("Ray at t=3: {}", r.at(3));

  return EXIT_SUCCESS;
}
