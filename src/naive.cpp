#include "frr/naive.hpp"
#include "frr/common.hpp"

auto frr::naive(std::uint8_t *const data,
                const Vector2 TL, const Vector2 BR,
                const std::size_t max_iteration) -> void
{
  constexpr double w              = static_cast<double>(frr::width);
  constexpr double h              = static_cast<double>(frr::height);
  constexpr double x0_factor      = 3.0 / w;
  constexpr double y0_factor      = 2.0 / h;
  constexpr double x_delta_factor = 3.0 / (w * w);
  constexpr double y_delta_factor = 2.0 / (h * h);

  const double x_delta  = (BR.x - TL.x) * x_delta_factor;
  const double y_delta  = (BR.y - TL.y) * y_delta_factor;
  const double x_origin = TL.x * x0_factor - 2.0;
  double       y0       = TL.y * y0_factor - 1.0;
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
