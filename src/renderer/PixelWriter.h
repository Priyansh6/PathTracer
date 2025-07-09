#ifndef PIXELWRITER_H
#define PIXELWRITER_H
#include "Colour.h"

#include <concepts>

template<typename T>
concept PixelWriter = requires(T renderer, int x, int y, const Colour& colour) {
  { renderer.init() } -> std::same_as<void>;
  { renderer.write_pixel(x, y, colour) } -> std::same_as<void>;
  { renderer.cleanup() } -> std::same_as<void>;
};

#endif// PIXELWRITER_H
