#ifndef FRR_SIMD_HPP_
#define FRR_SIMD_HPP_

#include "frr/utils/Vector.hpp"
#include <cstdint>

namespace frr
{
  auto simd(std::uint8_t *const data,
            const Vector_f64& TL, const Vector_f64& delta,
            const std::size_t max_iteration) -> void;

}
#endif // FRR_SIMD_HPP_