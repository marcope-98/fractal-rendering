#include "frr/impl/threadpool.hpp"
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
}

auto frr::Worker::run() -> void
{
  const __m256i one  = _mm256_set1_epi64x(1);
  const __m256d two  = _mm256_set1_pd(2.0);
  const __m256d four = _mm256_set1_pd(4.0);

  while (this->alive)
  {
    {
      std::unique_lock<std::mutex> lm{mtx};
      cv.wait(lm);
    }
    const __m256i max_iter = _mm256_set1_epi64x(this->max_iterations);

    const __m256d x_delta  = _mm256_set1_pd(this->delta.x);
    const __m256d y_delta  = _mm256_set1_pd(this->delta.y);
    const __m256d x_step   = _mm256_set1_pd(this->delta.x * 4.0);
    const __m256d x_origin = _mm256_add_pd(_mm256_set1_pd(this->TL.x),
                                           _mm256_mul_pd(x_delta, _mm256_set_pd(3.0, 2.0, 1.0, 0.0)));
    __m256d       y0       = _mm256_set1_pd(this->TL.y + this->row_start * this->delta.y);
    for (std::size_t row{this->row_start}; row < this->row_end; ++row)
    {
      __m256d x0 = x_origin;
      for (std::size_t col{}; col < frr::width; col += 4)
      {
        __m256d x = _mm256_setzero_pd(), y = _mm256_setzero_pd();
        __m256d x2 = _mm256_setzero_pd(), y2 = _mm256_setzero_pd();
        __m256i iteration = one;
        __m256i condition1, condition2;

      loop:
        y          = _mm256_fmadd_pd(two, _mm256_mul_pd(x, y), y0);
        x          = _mm256_add_pd(_mm256_sub_pd(x2, y2), x0);
        x2         = _mm256_mul_pd(x, x);
        y2         = _mm256_mul_pd(y, y);
        condition1 = _mm256_castpd_si256(_mm256_cmp_pd(_mm256_add_pd(x2, y2), four, _CMP_LE_OS));
        condition2 = _mm256_cmpgt_epi64(max_iter, iteration);
        condition2 = _mm256_and_si256(condition1, condition2);
        iteration  = _mm256_add_epi64(iteration, _mm256_and_si256(one, condition2));
        if (_mm256_movemask_pd(_mm256_castsi256_pd(condition2)) > 0)
          goto loop;

        this->data[row * frr::width + col + 0] = _mm256_extract_epi64(iteration, 0);
        this->data[row * frr::width + col + 1] = _mm256_extract_epi64(iteration, 1);
        this->data[row * frr::width + col + 2] = _mm256_extract_epi64(iteration, 2);
        this->data[row * frr::width + col + 3] = _mm256_extract_epi64(iteration, 3);
        x0                                     = _mm256_add_pd(x0, x_step);
      }
      y0 = _mm256_add_pd(y0, y_delta);
    }
    completed.fetch_add(1, std::memory_order::memory_order_release);
  }
}

auto frr::ThreadPool::init(std::uint32_t *const data) -> void
{
  constexpr static std::size_t rows_per_thread{frr::height / frr::n_threads};
  for (std::size_t i{}; i < frr::n_threads; ++i)
  {
    this->workers[i].alive     = true;
    this->workers[i].data      = data;
    this->workers[i].row_start = i * rows_per_thread;
    this->workers[i].row_end   = (i + 1) * rows_per_thread;
    this->workers[i].thread    = std::thread(&Worker::run, &this->workers[i]);
  }
}

auto frr::ThreadPool::run(const Vector_f64 &TL, const Vector_f64 &delta,
                          const std::size_t max_iterations) -> void
{
  for (std::size_t i{}; i < frr::n_threads; ++i)
    this->workers[i].start(TL, delta, max_iterations);
  completed.store(0, std::memory_order::memory_order_release);
  cv.notify_all();

  while (completed.load(std::memory_order::memory_order_acquire) < frr::n_threads)
    std::this_thread::yield();
}

auto frr::ThreadPool::shutdown() -> void
{
  for (std::size_t i{}; i < frr::n_threads; i++)
    this->workers[i].alive = false;
  cv.notify_all();

  for (std::size_t i{}; i < frr::n_threads; ++i)
    this->workers[i].thread.join();
}