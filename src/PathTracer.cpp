#include <cstdlib>
#include <print>

#include "Vec3.h"

int main()
{
  const Vec3 v1(3, 2, 1);
  const Vec3 v2(3, 3, 6);
  std::print("{0:} {0:}", v1 - v2);

  return EXIT_SUCCESS;
}
