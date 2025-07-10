#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "Colour.h"
#include "PpmWriter.h"
#include "WindowController.h"
#include "entities/Ray.h"
#include "entities/World.h"

// The Renderer class is responsible for rendering a scene defined by a World and a Camera.
// It can output the rendered image to a PPM file or display it in a window.
// The rendering process involves sampling rays from the camera, tracing them through the scene,
// and shading the resulting hits with materials defined in the World.
class Renderer
{
public:
  Renderer() = delete;
  explicit Renderer(int image_width, int image_height, const Camera& camera);

  Renderer(const Renderer& other) = delete;
  Renderer(Renderer&& other) noexcept = delete;
  Renderer& operator=(const Renderer& other) = delete;
  Renderer& operator=(Renderer&& other) noexcept = delete;
  ~Renderer() = default;

  void render_to_ppm(const World& world, int samples_per_pixel, int max_depth, PpmWriter& ppm_writer) const;
  void render_to_window(const World& world,
    int samples_per_pixel,
    int max_depth,
    WindowController& window_controller) const;

private:
  [[nodiscard]] Colour sample_pixel(int x, int y, const World& world, int samples_per_pixel, int max_depth) const;

  [[nodiscard]] static Colour trace_and_shade(const Ray& r, const World& world, int max_depth);

  int m_image_width;
  int m_image_height;
  const Camera& m_camera;
};


#endif// RENDERER_H
