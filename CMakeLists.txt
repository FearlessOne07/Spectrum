cmake_minimum_required(VERSION 3.20)
project("Spectrum" CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/Base")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
