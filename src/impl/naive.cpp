#include "frr/impl/naive.hpp"
#include "frr/common.hpp"

auto frr::naive(std::uint8_t *const data,
                const Vector_f64 &TL, const Vector_f64 &delta,
                const std::size_t max_iteration) -> void
{
  double x0{TL.x}, y0{TL.y};
  for (std::size_t row{}; row < frr::height; ++row)
  {
    x0 = TL.x;
    for (std::size_t col{}; col < frr::width; ++col)
    {
      double      x{}, y{}, x2{}, y2{};
      std::size_t iteration{};
      while (x2 + y2 <= 4 && iteration < max_iteration)
      {
        y  = 2 * x * y + y0;
        x  = x2 - y2 + x0;
        x2 = x * x;
        y2 = y * y;
        ++iteration;
      }
      data[row * frr::width + col] = frr::palette[iteration % 32u];
      x0 += delta.x;
    }
    y0 += delta.y;
  }
}
