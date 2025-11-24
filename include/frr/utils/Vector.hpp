#ifndef FRR_UTILS_VECTOR_HPP_
#define FRR_UTILS_VECTOR_HPP_
#include <cmath>
namespace frr
{
  template<class T>
  class Vector
  {
  public:
    T x{}, y{};

    [[nodiscard]] constexpr Vector        operator+(const Vector &other) const { return {this->x + other.x, this->y + other.y}; }
    [[nodiscard]] constexpr Vector        operator-(const Vector &other) const { return {this->x - other.x, this->y - other.y}; }
    [[nodiscard]] constexpr Vector        operator*(const Vector &other) const { return {this->x * other.x, this->y * other.y}; }
    [[nodiscard]] constexpr friend Vector operator*(T rhs, const Vector &lhs) { return {lhs.x * rhs, lhs.y * rhs}; }
    [[nodiscard]] constexpr Vector        operator*(T rhs) const { return {this->x * rhs, this->y * rhs}; }
    [[nodiscard]] constexpr Vector        operator/(T rhs) const { return {this->x / rhs, this->y / rhs}; }

    [[nodiscard]] constexpr Vector operator+() const { return *this; }
    [[nodiscard]] constexpr Vector operator-() const { return {-this->x, -this->y}; }

    constexpr Vector &operator+=(const Vector &rhs)
    {
      this->x += rhs.x;
      this->y += rhs.y;
      return *this;
    }

    constexpr Vector &operator-=(const Vector &rhs)
    {
      this->x -= rhs.x;
      this->y -= rhs.y;
      return *this;
    }
  };

  using Vector_i32 = Vector<int>;
  using Vector_f32 = Vector<float>;
  using Vector_f64 = Vector<double>;
} // namespace frr

#endif // FRR_UTILS_VECTOR_HPP_