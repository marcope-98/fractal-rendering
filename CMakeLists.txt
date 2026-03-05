cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(fractal_rendering VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

# Raylib target
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE CACHE INTERNAL "")
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.5
  GIT_PROGRESS   ON
  GIT_SHALLOW    1
  )
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)
target_compile_options(raylib PRIVATE "-w")

# FRactal Rendering (frr) library
find_package(Threads)
set(FRR_LIBRARY_NAME frr)
add_library(${FRR_LIBRARY_NAME}
  STATIC
  src/impl/naive.cpp
  src/impl/simd.cpp
  src/impl/threads.cpp
  src/impl/threadpool.cpp
  src/utils/Window.cpp
  src/utils/FractalRenderer.cpp
  src/utils/InputHandler.cpp
  src/utils/FractalRenderingManager.cpp
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
  PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
  -Wall -Wextra -Werror -Wpedantic -O2 -ffast-math -mavx -mavx2 -mfma>
)

target_link_libraries(${FRR_LIBRARY_NAME} 
  PUBLIC
  Threads::Threads raylib)

get_target_property(RAYLIB_INCLUDE_DIRS raylib INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(${FRR_LIBRARY_NAME} SYSTEM PRIVATE ${RAYLIB_INCLUDE_DIRS})

# Main Application target
add_executable(fractal_rendering main.cpp)
target_compile_options(fractal_rendering
  PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
  -Wall -Wextra -Werror -Wpedantic -O2>
  )

target_link_libraries(fractal_rendering PUBLIC frr::frr)