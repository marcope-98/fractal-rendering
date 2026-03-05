#include "frr/impl/simd.hpp"
#include "frr/common.hpp"

#include <immintrin.h>

auto frr::simd(std::uint32_t *const data,
               const Vector_f64 &TL, const Vector_f64 &delta,
               const std::size_t max_iteration,
               const std::size_t row_start, const std::size_t row_end) -> void
{
  const __m256i one      = _mm256_set1_epi64x(1);
  const __m256d two      = _mm256_set1_pd(2.0);
  const __m256d four     = _mm256_set1_pd(4.0);
  const __m256i max_iter = _mm256_set1_epi64x(max_iteration);

  const __m256d x_delta  = _mm256_set1_pd(delta.x);
  const __m256d y_delta  = _mm256_set1_pd(delta.y);
  const __m256d x_step   = _mm256_set1_pd(delta.x * 4.0);
  const __m256d x_origin = _mm256_add_pd(_mm256_set1_pd(TL.x),
                                         _mm256_mul_pd(x_delta, _mm256_set_pd(3.0, 2.0, 1.0, 0.0)));
  __m256d       y0       = _mm256_set1_pd(TL.y + row_start * delta.y);
  for (std::size_t row{row_start}; row < row_end; ++row)
  {
    __m256d x0 = x_origin;
    for (std::size_t col{}; col < frr::width; col += 4)
    {
      __m256d x = _mm256_setzero_pd(), y = _mm256_setzero_pd();
      __m256d x2 = _mm256_setzero_pd(), y2 = _mm256_setzero_pd();
      __m256i iteration = _mm256_setzero_si256();
      __m256i condition1, condition2;

      do
      {
        y          = _mm256_fmadd_pd(two, _mm256_mul_pd(x, y), y0);
        x          = _mm256_add_pd(_mm256_sub_pd(x2, y2), x0);
        x2         = _mm256_mul_pd(x, x);
        y2         = _mm256_mul_pd(y, y);
        condition1 = _mm256_castpd_si256(_mm256_cmp_pd(_mm256_add_pd(x2, y2), four, _CMP_LE_OS));
        condition2 = _mm256_cmpgt_epi64(max_iter, iteration);
        condition2 = _mm256_and_si256(condition1, condition2);
        iteration  = _mm256_add_epi64(iteration, _mm256_and_si256(one, condition2));
      } while (_mm256_movemask_pd(_mm256_castsi256_pd(condition2)) > 0);

      data[row * frr::width + col + 0] = static_cast<std::uint32_t>(_mm256_extract_epi64(iteration, 0));
      data[row * frr::width + col + 1] = static_cast<std::uint32_t>(_mm256_extract_epi64(iteration, 1));
      data[row * frr::width + col + 2] = static_cast<std::uint32_t>(_mm256_extract_epi64(iteration, 2));
      data[row * frr::width + col + 3] = static_cast<std::uint32_t>(_mm256_extract_epi64(iteration, 3));
      x0                               = _mm256_add_pd(x0, x_step);
    }
    y0 = _mm256_add_pd(y0, y_delta);
  }
}
