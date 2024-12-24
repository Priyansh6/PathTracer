#ifndef MATERIAL_H
#define MATERIAL_H

#include "Lambertian.h"
#include "Metal.h"

#include <variant>

using Material = std::variant<Lambertian, Metal>;

#endif// MATERIAL_H
