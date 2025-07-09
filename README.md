# PathTracer

## Description

This is a simple path tracer written in C++ based on the book "Ray Tracing in One Weekend" by Peter Shirley.
It has been adapted by me to use modern C++ features and to be more performant, modular and extensible.

## Features

Entities: Spheres\
Materials: Lambertian (diffuse), Dielectric (transparent), Metal (reflective)\
Scenes: Two scenes provided in `World.h`\
Camera: Positionable camera with adjustable field of view

## Default Scene Render

![Default Scene Render](images/default_scene.png)

## Build

Pre-requisites:

- CMake (>= 3.29)
- C++-23 compatible compiler (ideally msvc or clang on windows/clang on linux)
- On Linux if using clang: `clang-tools`, `libc++-dev`, `libc++abi-dev`

You can build the project automatically using JetBrains CLion, Windows Visual Studio or manually using CMake.
To build manually with CMake, run the following commands (for the example preset `windows-clang-release`) in the project
root directory:

```bash
cmake --preset "windows-clang-release"
cd out/build/windows-clang-release
cmake --build .
```

## Run

To run the path tracer, execute the following command in the build directory. It will output the rendered image in PPM
format to a file named `output.ppm`.

### In Linux

```bash
./PathTracer
```

### In Windows (PowerShell)

```bash
./PathTracer.exe
```