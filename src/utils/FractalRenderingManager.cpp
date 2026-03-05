#include "frr/utils/FractalRenderingManager.hpp"
#include <chrono>

auto frr::FractalRenderingManager::run() -> void
{
  while (!this->window.should_close())
  {
    this->apply(this->input.poll(this->renderer.max_iterations));
    auto t0 = std::chrono::steady_clock::now();
    this->renderer.render(this->camera);
    auto t1 = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    this->window.upload(this->renderer.data);
    this->window.draw(this->renderer.max_iterations, std::to_string(ms), method_names[static_cast<int>(this->renderer.method)]);
  }
}

auto frr::FractalRenderingManager::apply(const FrameInput &input) -> void
{
  if (input.reset_camera) this->camera.reset();
  if (input.pan_delta) this->camera.pan(*input.pan_delta);
  if (input.zoom_event) this->camera.zoom(input.zoom_event->first, input.zoom_event->second);
  if (input.method_change) this->renderer.method = *input.method_change;
  if (input.iteration_delta != 0) this->renderer.max_iterations += input.iteration_delta;
}
