#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "Colour.h"
#include "PixelWriter.h"
#include "entities/Ray.h"
#include "entities/World.h"

template<PixelWriter PixelWriter>
class Renderer
{
public:
  Renderer() = delete;
  explicit Renderer(int image_width, int image_height, const Camera& camera, PixelWriter& pixel_writer);

  Renderer(const Renderer& other) = delete;
  Renderer(Renderer&& other) noexcept = delete;
  Renderer& operator=(const Renderer& other) = delete;
  Renderer& operator=(Renderer&& other) noexcept = delete;
  ~Renderer() = default;

  void render(const World& world, int samples_per_pixel, int max_depth) const;

private:
  [[nodiscard]] Colour sample_pixel(int x, int y, const World& world, int samples_per_pixel, int max_depth) const;

  [[nodiscard]] static Colour trace_and_shade(const Ray& r, const World& world, int max_depth);

  int m_image_width;
  int m_image_height;
  const Camera& m_camera;
  PixelWriter& m_pixel_writer;
};


#endif// RENDERER_H
