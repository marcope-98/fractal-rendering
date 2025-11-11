cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(fractal_rendering VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_BUILD_TYPE Release)

# Raylib target
include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.5
  )
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)

# FRactal Rendering (frr) library
set(FRR_LIBRARY_NAME frr)
add_library(${FRR_LIBRARY_NAME}
  STATIC
  src/naive.cpp
  src/simd.cpp
)
add_library(${FRR_LIBRARY_NAME}::${FRR_LIBRARY_NAME} ALIAS ${FRR_LIBRARY_NAME})

target_include_directories(${FRR_LIBRARY_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(${FRR_LIBRARY_NAME}
  PUBLIC
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
  -Wall -Wextra -Wpedantic -O2 -march=native>
)

target_link_libraries(frr raylib)

# Main Application target
add_executable(fractal_rendering main.cpp)
target_link_libraries(fractal_rendering frr::frr raylib)