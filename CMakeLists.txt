cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(fractal_rendering VERSION 0.0.1 LANGUAGES C CXX)
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

# Raylib target
include(ImportRaylibTarget)

# FRactal Rendering (frr) library
include(ImportFRRTarget)
include(IgnoreExternalHeadersWarning)
ignore_external_headers_warning(${FRR_LIBRARY_NAME} raylib)

# Main Application target
add_executable(fractal_rendering main.cpp)
target_compile_options(fractal_rendering
  PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
  -Wall -Wextra -Werror -Wpedantic -O2>
  )

target_link_libraries(fractal_rendering frr::frr)