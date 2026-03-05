#include "frr/utils/FractalRenderer.hpp"

#include "frr/common.hpp"

#include "frr/impl/naive.hpp"
#include "frr/impl/simd.hpp"
#include "frr/impl/threads.hpp"
#include "frr/impl/threadpool.hpp"

#include "frr/utils/Camera.hpp"

frr::FractalRenderer::FractalRenderer()
{
  this->data = new std::uint32_t[frr::width * frr::height]();
  this->tp.init(this->data);
}

frr::FractalRenderer::~FractalRenderer()
{
  this->tp.shutdown();
  delete[] this->data;
}

auto frr::FractalRenderer::render(const Camera &camera) -> void
{
  auto tl    = camera.getTL();
  auto delta = camera.delta();
  switch (this->method)
  {
    case RenderMethod::Naive:
      frr::naive(this->data, tl, delta, this->max_iterations);
      break;
    case RenderMethod::AVX2:
      frr::simd(this->data, tl, delta, this->max_iterations);
      break;
    case RenderMethod::Threads:
      frr::threads(this->data, tl, delta, this->max_iterations);
      break;
    case RenderMethod::ThreadPool:
      this->tp.run(tl, delta, this->max_iterations);
      break;
  }
}