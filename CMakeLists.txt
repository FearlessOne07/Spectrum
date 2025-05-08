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

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/Base")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
