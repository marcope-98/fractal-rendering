#ifndef FRR_IMPL_THREADPOOL_HPP_
#define FRR_IMPL_THREADPOOL_HPP_

#include <atomic>
#include <cstdint>
#include <thread>

#include "frr/common.hpp"
#include "frr/utils/Vector.hpp"

namespace frr
{
  struct Worker
  {
    Vector_f64    delta{}, TL{};
    std::size_t   max_iterations{};
    bool          alive{true};
    std::size_t   row_start{}, row_end{};
    std::uint8_t *data{nullptr};

    std::thread             thread;

    auto start(const Vector_f64 &TL, const Vector_f64 &BR, const std::size_t max_iterations) -> void;
    auto run() -> void;
  };

  struct ThreadPool
  {
    Worker workers[frr::n_threads];

    auto init(std::uint8_t *const data) -> void;
    auto run(const Vector_f64 &TL, const Vector_f64 &BR, const std::size_t max_iterations) -> void;
    auto shutdown() -> void;
  };
} // namespace frr

#endif // FRR_IMPL_THREADPOOL_HPP_