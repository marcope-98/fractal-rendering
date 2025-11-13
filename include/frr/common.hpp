#ifndef FRR_COMMON_HPP_
#define FRR_COMMON_HPP_

#include <cstdint>

namespace frr
{
  inline constexpr std::size_t  width{1280};
  inline constexpr std::size_t  height{720};
  inline constexpr std::uint8_t palette[32] = {0, 15, 31, 46, 63, 78, 94, 109, 127, 142, 158, 173, 190, 205, 221, 236,
                                               255, 236, 221, 205, 190, 173, 158, 142, 127, 109, 94, 78, 63, 46, 31, 15};
  inline constexpr std::size_t  n_threads{36};
} // namespace frr
#endif // FRR_COMMON_HPP_