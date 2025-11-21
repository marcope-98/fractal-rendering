#ifndef FRR_NAIVE_HPP_
#define FRR_NAIVE_HPP_
#include <cstdint>

#include "frr/utils/Vector.hpp"

namespace frr
{
  auto naive(std::uint32_t *const data,
             const Vector_f64 &TL, const Vector_f64 &delta,
             const std::size_t max_iteration) -> void;

}

#endif