#ifndef FRR_UTILS_CAMERA_HPP_
#define FRR_UTILS_CAMERA_HPP_
#include "frr/common.hpp"
#include "frr/utils/Vector.hpp"

#include <algorithm>

namespace frr
{
  class Camera
  {
  public:
    Vector_f64 target{0.0, 0.0};
    Vector_f64 offset{0.0, 0.0};
    double     zoom_factor{1.0};

  public:
    void pan(const Vector_f64 &delta) { this->target -= (delta / this->zoom_factor); }
    void pan(Vector_f64 &&delta) { this->target -= (delta / this->zoom_factor); }

    void zoom(const Vector_f64 &mouse_position, double wheel)
    {
      Vector_f64 mouseWorldPos = this->screen2world(mouse_position);
      this->offset             = mouse_position;
      this->target             = mouseWorldPos;
      this->zoom_factor += this->zoom_factor * wheel * 0.1;
      this->zoom_factor = std::clamp(this->zoom_factor, 1.0, frr::zoom_limit);
    }

    void reset()
    {
      this->target      = {0.0, 0.0};
      this->offset      = {0.0, 0.0};
      this->zoom_factor = 1.0;
    }

    [[nodiscard]] Vector_f64 getTL() const { return this->screen2world(this->world_lower_bound) * this->m + this->q; }
    [[nodiscard]] Vector_f64 getBR() const { return this->screen2world(this->world_upper_bound) * this->m + this->q; }
    [[nodiscard]] Vector_f64 delta() const { return this->m / this->zoom_factor; }

    [[nodiscard]] Vector_f64 screen2world(const Vector_f64 &screen) const { return ((screen - this->offset) / this->zoom_factor) + this->target; }

  private:
    constexpr static Vector_f64 world_lower_bound{0.0, 0.0};
    constexpr static Vector_f64 world_upper_bound{frr::width, frr::height};
    constexpr static Vector_f64 m{frr::mx, frr::my};
    constexpr static Vector_f64 q{frr::qx, frr::qy};
  };
} // namespace frr

#endif // FRR_UTILS_CAMERA_HPP_