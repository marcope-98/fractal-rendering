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

    constexpr Vector        operator+(const Vector &other) const { return {this->x + other.x, this->y + other.y}; }
    constexpr Vector        operator-(const Vector &other) const { return {this->x - other.x, this->y - other.y}; }
    constexpr Vector        operator*(const Vector &other) const { return {this->x * other.x, this->y * other.y}; }
    constexpr friend Vector operator*(T rhs, const Vector &lhs) { return {lhs.x * rhs, lhs.y * rhs}; }
    constexpr Vector        operator*(T rhs) const { return {this->x * rhs, this->y * rhs}; }
    constexpr Vector        operator/(T rhs) const { return {this->x / rhs, this->y / rhs}; }

    constexpr Vector operator+() const { return *this; }
    constexpr Vector operator-() const { return {-this->x, -this->y}; }

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
  template<class T>
  inline Vector<T> max(const Vector<T> &lhs, const Vector<T> &rhs)
  {
    return {std::fmax(lhs.x, rhs.x), std::fmax(lhs.y, rhs.y)};
  }

  template<class T>
  inline Vector<T> min(const Vector<T> &lhs, const Vector<T> &rhs)
  {
    return {std::fmin(lhs.x, rhs.x), std::fmin(lhs.y, rhs.y)};
  }

  using Vector_i32 = Vector<int>;
  using Vector_f32 = Vector<float>;
  using Vector_f64 = Vector<double>;
} // namespace frr

#endif // FRR_UTILS_VECTOR_HPP_