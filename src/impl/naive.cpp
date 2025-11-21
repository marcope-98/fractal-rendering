#include "frr/impl/naive.hpp"
#include "frr/common.hpp"

auto frr::naive(std::uint8_t *const data,
                const Vector_f64 &TL, const Vector_f64 &BR,
                const std::size_t max_iteration) -> void
{
  constexpr double inv_w      = 1.0 / static_cast<double>(frr::width);
  constexpr double inv_h      = 1.0 / static_cast<double>(frr::height);

  const Vector_f64 delta = BR - TL;
  const double x_delta   = delta.x * inv_w;
  const double y_delta   = delta.y * inv_h;
  const double x_origin  = TL.x;
  double       y0        = TL.y;
  for (std::size_t row{}; row < frr::height; ++row)
  {
    double x0 = x_origin;
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
      x0 += x_delta;
    }
    y0 += y_delta;
  }
}
