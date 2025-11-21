#ifndef FRR_COMMON_HPP_
#define FRR_COMMON_HPP_

#include <cstdint>
#include <limits>

namespace frr
{
  inline constexpr std::size_t  width{1280};
  inline constexpr std::size_t  height{720};
  inline constexpr std::uint8_t palette[32] = {0, 15, 31, 46, 63, 78, 94, 109, 127, 142, 158, 173, 190, 205, 221, 236,
                                               255, 236, 221, 205, 190, 173, 158, 142, 127, 109, 94, 78, 63, 46, 31, 15};
  inline constexpr std::size_t  n_threads{36};

  inline constexpr double mandelbrot_min_x{-2.0};
  inline constexpr double mandelbrot_max_x{+1.0};
  inline constexpr double mandelbrot_min_y{-1.0};
  inline constexpr double mandelbrot_max_y{+1.0};
  inline constexpr double mandelbrot_size_x{mandelbrot_max_x - mandelbrot_min_x};
  inline constexpr double mandelbrot_size_y{mandelbrot_max_y - mandelbrot_min_y};
  
  inline constexpr double inv_w{1.0 / static_cast<double>(width)};
  inline constexpr double inv_h{1.0 / static_cast<double>(height)};
  inline constexpr double mx{mandelbrot_size_x * inv_w};
  inline constexpr double my{mandelbrot_size_y * inv_h};
  inline constexpr double qx{mandelbrot_min_x};
  inline constexpr double qy{mandelbrot_min_y};

  inline constexpr double zoom_limit{(mandelbrot_size_x * inv_w) / (std::numeric_limits<double>::epsilon())};

} // namespace frr
#endif // FRR_COMMON_HPP_