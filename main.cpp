#include <chrono>
#include <cstdint>
#include <string>

#include "raylib.h"
#include "raymath.h"

#include "frr/common.hpp"
#include "frr/naive.hpp"
#include "frr/simd.hpp"

class CameraManager
{
public:
  Camera2D camera{};

  CameraManager()
  {
    this->camera.offset   = {0.f, 0.f};
    this->camera.target   = {0.f, 0.f};
    this->camera.rotation = 0.f;
    this->camera.zoom     = 1.f;
  }

  void pan()
  {
    Vector2 delta       = GetMouseDelta();
    delta               = Vector2Scale(delta, -1.f / this->camera.zoom);
    this->camera.target = Vector2Add(this->camera.target, delta);
  }

  void zoom(float wheel)
  {
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), this->camera);
    this->camera.offset   = GetMousePosition();
    this->camera.target   = mouseWorldPos;
    float scale           = 0.2f * wheel;
    this->camera.zoom     = fmaxf(expf(logf(this->camera.zoom) + scale), 1.f);
  }

  void bound()
  {
    Vector2 boundary;
    boundary            = GetScreenToWorld2D(this->world_lower_bound, camera);
    boundary            = Vector2Min(boundary, this->world_lower_bound);
    this->camera.target = Vector2Subtract(this->camera.target, boundary);

    boundary            = GetScreenToWorld2D(this->world_upper_bound, camera);
    boundary            = Vector2Subtract(Vector2Max(boundary, this->world_upper_bound), this->world_upper_bound);
    this->camera.target = Vector2Subtract(this->camera.target, boundary);
  }

  Vector2 getTL() const { return GetScreenToWorld2D(this->world_lower_bound, camera); }
  Vector2 getBR() const { return GetScreenToWorld2D(this->world_upper_bound, camera); }

  void reset()
  {
    this->camera.zoom   = 1.f;
    this->camera.offset = {0.f, 0.f};
    this->camera.target = {0.f, 0.f};
  }

private:
  const Vector2 world_lower_bound{0.f, 0.f};
  const Vector2 world_upper_bound{frr::width, frr::height};
};

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
  CameraManager cm;
  Texture2D     texture          = LoadTextureFromImage(image);
  int           last_key_pressed = KEY_NULL;
  while (!WindowShouldClose())
  {
    // User Input
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) cm.reset();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) cm.pan();
    if (float wheel = GetMouseWheelMove(); wheel != 0) cm.zoom(wheel);
    cm.bound();
    if (IsKeyPressed(KEY_KP_ADD)) max_iterations += 32;
    if (IsKeyPressed(KEY_KP_SUBTRACT)) max_iterations = max_iterations == 32 ? 32 : max_iterations - 32;
    if (int key = GetKeyPressed(); KEY_ONE <= key && key <= KEY_TWO) last_key_pressed = key;
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
      default:
        frr::naive(data, cm.getTL(), cm.getBR(), max_iterations);
    }
    auto end = std::chrono::steady_clock::now();
    // Update Texture with new screen buffer
    UpdateTexture(texture, data);
    // Draw Texture
    BeginDrawing();
    DrawTexture(texture, 0, 0, WHITE);
    DrawText((std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) + " [ms] | " + std::to_string(max_iterations)).c_str(),
             0, 0, 20, RAYWHITE);
    EndDrawing();
  }

  // Clean up
  UnloadTexture(texture);
  CloseWindow();
  delete[] data;
  return 0;
}