#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include "Colour.h"
#include "Tile.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <vector>

// The WindowController class manages the SDL window and renderer.
// It initializes the window, handles events, and updates the display with rendered tiles.
// It is designed to work with the Renderer class to display the rendered image in a window.
// The window is only alive for the lifetime of the WindowController instance.
class WindowController
{
public:
  WindowController() = delete;
  explicit WindowController(int width, int height);

  ~WindowController();
  WindowController(const WindowController& other) = delete;
  WindowController(WindowController&& other) noexcept = delete;
  WindowController& operator=(const WindowController& other) = delete;
  WindowController& operator=(WindowController&& other) noexcept = delete;

  [[nodiscard]] bool init_window();
  static bool poll_events_until_quit();// Returns true if the quit event was received.
  static void wait_for_quit();
  void update_tile(const Tile& tile, const std::vector<RGBA>& buffer);
  void present();

private:
  int m_width;
  int m_height;
  SDL_Window* m_window{ nullptr };
  SDL_Renderer* m_renderer{ nullptr };
  SDL_Texture* m_texture{ nullptr };
};

#endif// WINDOWCONTROLLER_H
