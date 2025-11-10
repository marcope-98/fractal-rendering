#include "frr/naive.hpp"
#include "frr/common.hpp"

#include <cmath>

auto frr::naive(uint8_t *const data,
                const Vector2 TL, const Vector2 BR,
                const std::size_t max_iteration) -> void
{
  const double w        = (double)frr::width;
  const double h        = (double)frr::height;
  const double x_delta  = (BR.x - TL.x) * 3.0 / (w * w);
  const double y_delta  = (BR.y - TL.y) * 2.0 / (h * h);
  const double x_origin = (TL.x * 3.0 / w) - 2.0;
  double       y0       = (TL.y * 2.0 / h) - 1.0;
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
      data[row * frr::width + col] = iteration % 255u;
      x0 += x_delta;
    }
    y0 += y_delta;
  }
}
