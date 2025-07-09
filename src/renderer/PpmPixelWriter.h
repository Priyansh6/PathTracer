#ifndef PPMPIXELWRITER_H
#define PPMPIXELWRITER_H

#include "Colour.h"

#include <cstddef>
#include <ostream>
#include <vector>

class PpmPixelWriter
{
public:
  PpmPixelWriter() = delete;
  explicit PpmPixelWriter(int width, int height, std::ostream& out);

  PpmPixelWriter(const PpmPixelWriter& other) = delete;
  PpmPixelWriter(PpmPixelWriter&& other) noexcept = delete;
  PpmPixelWriter& operator=(const PpmPixelWriter& other) = delete;
  PpmPixelWriter& operator=(PpmPixelWriter&& other) noexcept = delete;
  ~PpmPixelWriter() = default;

  void init();
  void write_pixel(int x, int y, const Colour& colour);
  static void cleanup();

private:
  int m_width;
  int m_height;
  std::size_t m_current_pixel{ 0 };
  std::vector<Colour> m_pixels;
  std::ostream& m_out;
};

#endif// PPMPIXELWRITER_H
