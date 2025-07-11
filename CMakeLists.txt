cmake_minimum_required(VERSION 3.29)
project(PathTracer)

# Require C++23 and disable extensions for all targets 
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.16
        SYSTEM
)

FetchContent_Declare(
        oneTBB
        GIT_REPOSITORY https://github.com/uxlfoundation/oneTBB.git
        GIT_TAG v2022.2.0
        SYSTEM
)

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v3.3.1
        SYSTEM
)

set(TBB_TEST OFF CACHE BOOL "Disable TBB tests")

FetchContent_MakeAvailable(SDL3 oneTBB cxxopts)

# Suppress '-Wshadow' warning for oneTBB which causes errors with -Werror
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(tbb PRIVATE -Wno-shadow)
    target_compile_options(tbbmalloc PRIVATE -Wno-shadow)
endif()

set(SOURCES
        src/Main.cpp
        src/entities/Vec3.h
        src/entities/Ray.h
        src/entities/Ray.cpp
        src/Config.h
        src/renderer/Colour.h
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
        src/renderer/PpmWriter.cpp
        src/renderer/PpmWriter.h
        src/renderer/WindowController.cpp
        src/renderer/WindowController.h
        src/renderer/Tile.h
)

add_executable(PathTracer ${SOURCES})

target_link_libraries(PathTracer PRIVATE SDL3::SDL3 TBB::tbb cxxopts::cxxopts)
target_include_directories(PathTracer PRIVATE "${CMAKE_SOURCE_DIR}/src")

# Copy DLLs on Windows
if (WIN32)
    add_custom_command(
            TARGET PathTracer
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:PathTracer>
            COMMENT "Copying SDL3.dll to executable directory"
    )

    add_custom_command(
            TARGET PathTracer
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TBB::tbb> $<TARGET_FILE_DIR:PathTracer>
            COMMENT "Copying tbb.dll to executable directory"
    )
endif ()
