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

project("PathTracer")

set(SOURCES
        src/PathTracer.cpp
        src/Vec3.h
        src/Ray.h
        src/Ray.cpp
        src/Config.h
        src/Colour.h
        src/Colour.cpp
        src/Camera.h
        src/Hittable.h
        src/Sphere.h
        src/Sphere.cpp
        src/Utils.h
        src/World.h
)

# Add source to this project's executable.
add_executable(PathTracer ${SOURCES})
