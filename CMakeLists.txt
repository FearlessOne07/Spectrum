cmake_minimum_required(VERSION 3.20)
project("Spectrum" CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ---------------------------
# Compiler-specific flags
# ---------------------------
if(MSVC)
  # MSVC flags
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG   "/Zi /Od /DDEBUG")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG")
else()
  # GCC/Clang flags
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto=auto -DNDEBUG")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
endif()

# Enable Wayland backend in GLFW
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "Build GLFW with Wayland support")

# Disable X11 backend in GLFW
set(GLFW_BUILD_X11 OFF CACHE BOOL "Do not build GLFW with X11 support")

# Set the graphics API (assuming this is a custom variable)
set(GRAPHICS GRAPHICS_API_OPENGL_43 CACHE STRING "Select the graphics API")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/Base")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
