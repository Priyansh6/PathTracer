#ifndef COLOUR_H
#define COLOUR_H

#include "Vec3.h"

#include <iostream>

using Colour = Vec3;

namespace colour {
constexpr int maximum_value = 255;
}

// Writes the colour to the output stream in the format "r g b\n" given a Colour vector containing values in the
// range [0,1].
void write_colour(std::ostream& out, const Colour& pixel_colour);

#endif// COLOUR_H
