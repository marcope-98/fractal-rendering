#ifndef FRR_INPUTHANDLER_HPP_
#define FRR_INPUTHANDLER_HPP_

#include "frr/utils/Vector.hpp"
#include "frr/utils/FractalRenderer.hpp"

#include <optional>


namespace frr
{
  struct FrameInput
  {
    std::optional<Vector_f64>                    pan_delta;
    std::optional<std::pair<Vector_f64, double>> zoom_event;
    bool                                         reset_camera{false};
    std::optional<RenderMethod>                  method_change;
    int                                          iteration_delta{0};
  };

  struct InputHandler
  {
    auto poll(std::size_t current_max_iterations) const -> FrameInput;
  };
} // namespace frr

#endif // FRR_INPUTHANDLER_HPP_