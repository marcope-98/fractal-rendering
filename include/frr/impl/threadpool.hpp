#ifndef FRR_IMPL_THREADPOOL_HPP_
#define FRR_IMPL_THREADPOOL_HPP_

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thread>

#include "frr/common.hpp"
#include "frr/utils/Vector.hpp"

namespace frr
{
  struct Worker
  {
    Vector_f64    BR{}, TL{};
    std::size_t   max_iterations{};
    bool          alive{true};
    std::size_t   row_start{}, row_end{};
    std::uint8_t *data{nullptr};

    std::mutex              mtx;
    std::condition_variable cv;
    std::thread             thread;

    void start(const Vector_f64 &TL, const Vector_f64 &BR, const std::size_t max_iterations);
    void run();
  };

  struct ThreadPool
  {
    Worker workers[frr::n_threads];

    explicit ThreadPool(std::uint8_t *const data);

    auto run(const Vector_f64 &TL, const Vector_f64 &BR, const std::size_t max_iterations) -> void;
    auto shutdown() -> void;
  };
} // namespace frr

#endif // FRR_IMPL_THREADPOOL_HPP_