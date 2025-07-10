#include "PpmWriter.h"
#include "Colour.h"

#include <ostream>
#include <vector>

PpmWriter::PpmWriter(const int width, const int height, std::ostream& out)
  : m_width{ width }, m_height{ height }, m_out{ out }
{}

void PpmWriter::write_buffer(const std::vector<RGBA>& buffer)
{
  std::println(m_out, "P3\n{} {}\n{}", m_width, m_height, colour::colour_range - 1);
  for (const RGBA& pixel : buffer) {
    const auto& [r, g, b, a] = pixel.get_components();
    std::println(m_out, "{} {} {}", r, g, b);
  }
}