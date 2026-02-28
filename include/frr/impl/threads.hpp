#ifndef FRR_IMPL_THREADS_HPP_
#define FRR_IMPL_THREADS_HPP_

#include <cstdint>
#include <cstddef>

#include "frr/utils/Vector.hpp"


namespace frr
{
  auto threads(std::uint32_t *const data,
               const Vector_f64 &TL, const Vector_f64 &delta,
               const std::size_t max_iteration) -> void;
}

#endif // FRR_IMPL_THREADS_HPP_