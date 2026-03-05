#include "frr/impl/threadpool.hpp"
#include "frr/impl/simd.hpp"

#include <immintrin.h>

#include <condition_variable>
#include <mutex>

std::mutex               mtx;
std::condition_variable  cv;
std::atomic<std::size_t> completed{0};

auto frr::Worker::start(const Vector_f64 &TL, const Vector_f64 &delta,
                        const std::size_t max_iterations) -> void
{
  this->TL             = TL;
  this->delta          = delta;
  this->max_iterations = max_iterations;
  this->ready          = true;
}

auto frr::Worker::run() -> void
{
  while (this->alive.load(std::memory_order_relaxed))
  {
    {
      std::unique_lock<std::mutex> lm{mtx};
      cv.wait(lm, [this]
              { return this->ready || !this->alive; });
      this->ready = false;
    }
    frr::simd(this->data,
              this->TL, this->delta,
              this->max_iterations,
              this->row_start, this->row_end);
    completed.fetch_add(1, std::memory_order::memory_order_release);
  }
}

auto frr::ThreadPool::init(std::uint32_t *const data) -> void
{
  static_assert(frr::height % frr::n_threads == 0, "frr::height must be exactly divisible by frr::n_threads");
  constexpr static std::size_t rows_per_thread{frr::height / frr::n_threads};
  for (std::size_t i{}; i < frr::n_threads; ++i)
  {
    this->workers[i].data      = data;
    this->workers[i].row_start = i * rows_per_thread;
    this->workers[i].row_end   = (i + 1) * rows_per_thread;
    this->workers[i].thread    = std::thread(&Worker::run, &this->workers[i]);
  }
}

auto frr::ThreadPool::run(const Vector_f64 &TL, const Vector_f64 &delta,
                          const std::size_t max_iterations) -> void
{
  {
    std::unique_lock<std::mutex> lm{mtx};
    for (std::size_t i{}; i < frr::n_threads; ++i)
      this->workers[i].start(TL, delta, max_iterations);
  }
  completed.store(0, std::memory_order::memory_order_release);
  cv.notify_all();

  while (completed.load(std::memory_order::memory_order_acquire) < frr::n_threads)
    std::this_thread::yield();
}

auto frr::ThreadPool::shutdown() -> void
{
  for (std::size_t i{}; i < frr::n_threads; i++)
    this->workers[i].alive.store(false, std::memory_order_relaxed);
  cv.notify_all();

  for (std::size_t i{}; i < frr::n_threads; ++i)
    this->workers[i].thread.join();
}