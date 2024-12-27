#ifndef MATERIAL_H
#define MATERIAL_H

#include "Dielectric.h"
#include "Lambertian.h"
#include "Metal.h"

#include <variant>

using Material = std::variant<Lambertian, Metal, Dielectric>;

#endif// MATERIAL_H
