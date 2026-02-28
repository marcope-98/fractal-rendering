#ifndef FRR_FRACTALRENDERER_HPP_
#define FRR_FRACTALRENDERER_HPP_
#include <cstdint>

#include "frr/impl/threadpool.hpp"

namespace frr
{
  class Camera;

  enum class RenderMethod
  {
    Naive,
    AVX2,
    Threads,
    ThreadPool
  };

  struct FractalRenderer
  {
    FractalRenderer();
    ~FractalRenderer();
    auto render(const Camera &camera) -> void;

    std::uint32_t *data{nullptr};
    std::size_t    max_iterations{64};
    RenderMethod   method{RenderMethod::Naive};

  private:
    frr::ThreadPool tp;
  };
} // namespace frr

#endif // FRR_FRACTALRENDERER_HPP_