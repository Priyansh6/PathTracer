# CMakeList.txt : CMake project for PathTracer, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.29)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

# Require C++23 and disable extensions for all targets 
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(PathTracer)

set(SOURCES
        src/PathTracer.cpp
        src/entities/Vec3.h
        src/entities/Ray.h
        src/entities/Ray.cpp
        src/Config.h
        src/renderer/Colour.h
        src/renderer/Colour.cpp
        src/renderer/Camera.h
        src/renderer/Camera.cpp
        src/entities/Hittable.h
        src/entities/Sphere.h
        src/entities/Sphere.cpp
        src/Utils.h
        src/Utils.cpp
        src/entities/World.h
        src/entities/World.cpp
        src/materials/Material.h
        src/materials/Lambertian.h
        src/materials/Metal.h
        src/materials/Scatter.cpp
        src/materials/Scatter.h
        src/materials/Dielectric.h
)

# Add source to this project's executable.
add_executable(PathTracer ${SOURCES})

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
target_include_directories(PathTracer PRIVATE ${SRC_DIR})
