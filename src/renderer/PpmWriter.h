#ifndef PPMWRITER_H
#define PPMWRITER_H

#include "Colour.h"

#include <ostream>
#include <vector>

class PpmWriter
{
public:
  PpmWriter() = delete;
  explicit PpmWriter(int width, int height, std::ostream& out);

  PpmWriter(const PpmWriter& other) = delete;
  PpmWriter(PpmWriter&& other) noexcept = delete;
  PpmWriter& operator=(const PpmWriter& other) = delete;
  PpmWriter& operator=(PpmWriter&& other) noexcept = delete;
  ~PpmWriter() = default;

  void write_buffer(const std::vector<RGBA>& buffer);

private:
  int m_width;
  int m_height;
  std::ostream& m_out;
};

#endif// PPMWRITER_H
