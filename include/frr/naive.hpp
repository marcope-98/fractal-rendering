#ifndef FRR_NAIVE_HPP_
#define FRR_NAIVE_HPP_
#include <cstdint>

#include "raylib.h"
namespace frr
{
  auto naive(uint8_t *const data,
             const Vector2 TL, const Vector2 BR,
             const std::size_t max_iteration) -> void;

}

#endif