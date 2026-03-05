#include "frr/utils/InputHandler.hpp"

#include "raylib.h"

auto frr::InputHandler::poll(std::size_t current_max_iterations) const -> FrameInput
{
  FrameInput input;
  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) input.reset_camera = true;
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
  {
    Vector2 delta   = GetMouseDelta();
    input.pan_delta = Vector_f64{delta.x, delta.y};
  }
  if (float wheel = GetMouseWheelMove(); wheel != 0)
  {
    Vector2 mp       = GetMousePosition();
    input.zoom_event = {Vector_f64{mp.x, mp.y}, static_cast<double>(wheel)};
  }

  if (IsKeyPressed(KEY_KP_ADD))
    input.iteration_delta = 32;
  else if (IsKeyPressed(KEY_KP_SUBTRACT) && current_max_iterations > 32)
    input.iteration_delta = -32;

  if (int key = GetKeyPressed(); KEY_ONE <= key && key <= KEY_FOUR)
    input.method_change = static_cast<RenderMethod>(key - KEY_ONE);
  return input;
}