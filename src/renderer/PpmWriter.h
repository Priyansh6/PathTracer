#ifndef PPMWRITER_H
#define PPMWRITER_H

#include "Colour.h"

#include <ostream>
#include <vector>

// The PpmWriter class is responsible for writing pixel data to a PPM (Portable Pixmap) file.
// It takes the image dimensions and an output stream, and provides a method to write a buffer of RGBA pixel data
// in the PPM format, specifically in the ASCII P3 format.
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
