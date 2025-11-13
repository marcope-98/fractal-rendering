#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include "raylib.h"

#include "frr/common.hpp"
#include "frr/impl/naive.hpp"
#include "frr/impl/simd.hpp"
#include "frr/impl/threadpool.hpp"
#include "frr/impl/threads.hpp"
#include "frr/utils/Camera.hpp"

int main(void)
{
  std::size_t max_iterations{64};
  uint8_t *   data  = new uint8_t[frr::width * frr::height]();
  Image       image = {
      data,                               /* data */
      frr::width,                         /* width */
      frr::height,                        /* height */
      PIXELFORMAT_UNCOMPRESSED_GRAYSCALE, /* format */
      1                                   /* mipmaps */
  };

  InitWindow(frr::width, frr::height, "Fractal Rendering");

  frr::Camera     cm;
  frr::ThreadPool tp(data);
  Texture2D       texture          = LoadTextureFromImage(image);
  int             last_key_pressed = KEY_NULL;
  while (!WindowShouldClose())
  {
    // User Input
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) cm.reset();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
      Vector2 delta = GetMouseDelta();
      cm.pan({delta.x, delta.y});
    }
    if (float wheel = GetMouseWheelMove(); wheel != 0)
    {
      Vector2 mousePosition = GetMousePosition();
      cm.zoom({mousePosition.x, mousePosition.y}, static_cast<double>(wheel));
    }
    cm.bound();

    if (IsKeyPressed(KEY_KP_ADD)) max_iterations += 32;
    if (IsKeyPressed(KEY_KP_SUBTRACT)) max_iterations = max_iterations == 32 ? 32 : max_iterations - 32;
    if (int key = GetKeyPressed(); KEY_ONE <= key && key <= KEY_FOUR) last_key_pressed = key;

    // Fractal computation
    auto begin = std::chrono::steady_clock::now();
    switch (last_key_pressed)
    {
      case KEY_ONE:
        frr::naive(data, cm.getTL(), cm.getBR(), max_iterations);
        break;
      case KEY_TWO:
        frr::simd(data, cm.getTL(), cm.getBR(), max_iterations);
        break;
      case KEY_THREE:
        frr::threads(data, cm.getTL(), cm.getBR(), max_iterations);
        break;
      case KEY_FOUR:
        tp.run(cm.getTL(), cm.getBR(), max_iterations);
        break;
      default:
        frr::naive(data, cm.getTL(), cm.getBR(), max_iterations);
    }
    auto end = std::chrono::steady_clock::now();
    // Update Texture with new screen buffer
    UpdateTexture(texture, data);
    // Draw Texture
    BeginDrawing();
    DrawTexture(texture, 0, 0, WHITE);
    DrawText((std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) + " [ms] | Max Iterations: " + std::to_string(max_iterations)).c_str(),
             0, 0, 20, RAYWHITE);
    EndDrawing();
  }

  tp.destroy();
  // Clean up
  UnloadTexture(texture);
  CloseWindow();
  delete[] data;
  return 0;
}