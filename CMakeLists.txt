cmake_minimum_required(VERSION 3.29)

include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.16
        SYSTEM
)
FetchContent_MakeAvailable(SDL3)

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
        src/Main.cpp
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
        src/materials/Material.h
        src/materials/Lambertian.h
        src/materials/Metal.h
        src/materials/Scatter.cpp
        src/materials/Scatter.h
        src/materials/Dielectric.h
        src/renderer/Renderer.cpp
        src/renderer/Renderer.h
        src/renderer/PixelWriter.h
        src/renderer/PpmPixelWriter.cpp
        src/renderer/PpmPixelWriter.h
)

add_executable(PathTracer ${SOURCES})

target_link_libraries(PathTracer PRIVATE SDL3::SDL3)

add_custom_command(
        TARGET PathTracer
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:PathTracer>
        COMMENT "Copying SDL3.dll to executable directory"
)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
target_include_directories(PathTracer PRIVATE ${SRC_DIR})