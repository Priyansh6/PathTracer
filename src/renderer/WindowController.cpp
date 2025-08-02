#include "WindowController.h"
#include "Colour.h"
#include "Config.h"
#include "Tile.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <algorithm>
#include <cstddef>
#include <span>

WindowController::WindowController(const int width, const int height) : m_width{ width }, m_height{ height } {}

WindowController::~WindowController()
{
  if (m_texture != nullptr) { SDL_DestroyTexture(m_texture); }
  if (m_renderer != nullptr) { SDL_DestroyRenderer(m_renderer); }
  if (m_window != nullptr) { SDL_DestroyWindow(m_window); }
  SDL_Quit();
}

bool WindowController::init_window()
{
  SDL_SetAppMetadata(app_metadata::name.c_str(), app_metadata::version.c_str(), app_metadata::id.c_str());
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());// NOLINT(*-vararg)
    return false;
  }
  if (!SDL_CreateWindowAndRenderer(window_config::title.c_str(), m_width, m_height, 0, &m_window, &m_renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());// NOLINT(*-vararg)
    return false;
  }

  m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);
  if (m_texture == nullptr) {
    SDL_Log("Couldn't create texture: %s", SDL_GetError());// NOLINT(*-vararg)
    return false;
  }
  return true;
}

bool WindowController::poll_events_until_quit()
{
  SDL_Event event{};
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) { return true; }
  }
  return false;
}

void WindowController::wait_for_quit()
{
  SDL_Event event{};
  while (SDL_WaitEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) { return; }
  }
}

void WindowController::update_tile(const Tile& tile, const int tile_size, const std::span<const RGBA> buffer)
{
  // Define the rectangular area of the tile to lock.
  const SDL_Rect lock_rect{
    .x = tile.x, .y = tile.y, .w = std::min(tile_size, m_width - tile.x), .h = std::min(tile_size, m_height - tile.y)
  };

  if (lock_rect.w <= 0 || lock_rect.h <= 0) { return; }

  void* texture_pixels{ nullptr };
  int pitch{};

  if (!SDL_LockTexture(m_texture, &lock_rect, &texture_pixels, &pitch)) { return; }
  const std::size_t row_elems = pitch / sizeof(RGBA);
  const auto texture_span = std::span{ static_cast<RGBA*>(texture_pixels), row_elems * lock_rect.h };

  // Copy the tile's pixel data from the buffer to the texture.
  for (int y_offset = 0; y_offset < lock_rect.h; y_offset++) {
    const int src_y = tile.y + y_offset;
    const int src_offset = (src_y * m_width) + tile.x;
    const std::span<const RGBA> src_span = buffer.subspan(src_offset, lock_rect.w);

    const int dst_offset = y_offset * static_cast<int>(row_elems);

    std::ranges::copy(src_span, texture_span.begin() + dst_offset);
  }
  SDL_UnlockTexture(m_texture);
}

void WindowController::present()
{
  SDL_RenderClear(m_renderer);
  SDL_RenderTexture(m_renderer, m_texture, nullptr, nullptr);
  SDL_RenderPresent(m_renderer);
}