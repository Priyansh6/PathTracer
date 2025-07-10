#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include "Colour.h"
#include "Tile.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <vector>

class WindowController
{
public:
  WindowController() = delete;
  explicit WindowController(int width, int height);

  [[nodiscard]] bool init();
  void present_tile(const Tile& tile, const std::vector<RGBA>& buffer);

private:
  int m_width;
  int m_height;
  SDL_Window* m_window{ nullptr };
  SDL_Renderer* m_renderer{ nullptr };
  SDL_Texture* m_texture{ nullptr };
};

#endif// WINDOWCONTROLLER_H
