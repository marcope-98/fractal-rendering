
find_package(Threads)

set(FRR_LIBRARY_NAME frr)
add_library(${FRR_LIBRARY_NAME}
  STATIC
  src/impl/naive.cpp
  src/impl/simd.cpp
  src/impl/threads.cpp
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
  -Wall -Wextra -Werror -Wpedantic -O2 -march=native>
)

target_link_libraries(${FRR_LIBRARY_NAME} Threads::Threads)