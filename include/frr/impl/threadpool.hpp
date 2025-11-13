#ifndef FRR_IMPL_THREADPOOL_HPP_
#define FRR_IMPL_THREADPOOL_HPP_

#include <cstdint>
#include <thread>

#include "frr/common.hpp"
#include "frr/utils/Barrier.hpp"
#include "frr/utils/Vector.hpp"

namespace frr
{
  class ThreadPool
  {
  public:
    explicit ThreadPool(std::uint8_t *const data);

    void run(const frr::Vector_f64 &TL, const frr::Vector_f64 &BR, std::size_t max_iterations);
    void destroy();

  private:
    Barrier         sync_start{frr::n_threads + 1}, sync_end{frr::n_threads + 1};
    std::thread     threads[frr::n_threads];
    std::size_t     max_iterations{};
    frr::Vector_f64 TL{}, BR{};
    bool            exit_triggered{false};
  };
} // namespace frr

#endif // FRR_IMPL_THREADPOOL_HPP_