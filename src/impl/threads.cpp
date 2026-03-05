#include "frr/impl/threads.hpp"

#include "frr/common.hpp"
#include "frr/impl/simd.hpp"

#include <thread>

auto frr::threads(std::uint32_t *const data,
                  const Vector_f64 &TL, const Vector_f64 &delta,
                  const std::size_t max_iteration) -> void
{
  static_assert(frr::height % frr::n_threads == 0, "frr::height must be exactly divisible by frr::n_threads");
  constexpr static std::size_t rows_per_thread{frr::height / frr::n_threads};
  std::thread threads[frr::n_threads];
  for (std::size_t i{}; i < frr::n_threads; ++i)
    threads[i] = std::thread(&frr::simd,
                             data,
                             TL, delta,
                             max_iteration,
                             i * rows_per_thread, (i + 1) * rows_per_thread);

  for (std::size_t i{}; i < frr::n_threads; ++i)
    threads[i].join();
}