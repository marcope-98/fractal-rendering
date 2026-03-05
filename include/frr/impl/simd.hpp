#ifndef FRR_SIMD_HPP_
#define FRR_SIMD_HPP_

#include <cstddef>
#include <cstdint>

#include "frr/common.hpp"
#include "frr/utils/Vector.hpp"

namespace frr
{
  auto simd(std::uint32_t *const data,
            const Vector_f64 &TL, const Vector_f64 &delta,
            const std::size_t max_iteration,
            const std::size_t row_start = 0, const std::size_t row_end = frr::height) -> void;

}
#endif // FRR_SIMD_HPP_