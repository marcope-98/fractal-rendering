cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(fractal_rendering VERSION 0.0.1 LANGUAGES C CXX)
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Release)

# Raylib target
include(ImportRaylibTarget)

# FRactal Rendering (frr) library
include(ImportFRRTarget)

# Main Application target
include(IgnoreExternalHeadersWarning)
add_executable(fractal_rendering main.cpp)
target_compile_options(fractal_rendering
  PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
  -Wall -Wextra -Werror -Wpedantic -O2>
  )

ignore_external_headers_warning(fractal_rendering raylib)
target_link_libraries(fractal_rendering frr::frr raylib)