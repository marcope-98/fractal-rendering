#include <cstdint>

#include "raylib.h"
#include "raymath.h"

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

  void reset()
  {
    this->camera.zoom   = 1.f;
    this->camera.offset = {0.f, 0.f};
    this->camera.target = {0.f, 0.f};
  }

private:
  const Vector2 world_lower_bound{0.f, 0.f};
  const Vector2 world_upper_bound{(float)GetScreenWidth(), (float)GetScreenHeight()};
};

int main(void)
{
  constexpr std::size_t width  = 1280;
  constexpr std::size_t height = 720;
  uint8_t *             data   = new uint8_t[width * height]();
  Image                 image  = {
      data,                               /* data */
      width,                              /* width */
      height,                             /* height */
      PIXELFORMAT_UNCOMPRESSED_GRAYSCALE, /* format */
      1 /* mipmaps */};

  InitWindow(width, height, "Fractal Rendering");
  CameraManager cm;
  Texture2D     texture = LoadTextureFromImage(image);
  while (!WindowShouldClose())
  {
    // User Input
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) cm.reset();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) cm.pan();
    if (float wheel = GetMouseWheelMove(); wheel != 0) cm.zoom(wheel);
    cm.bound();

    // Fractal computation

    // Update Texture with new screen buffer
    UpdateTexture(texture, (unsigned char *)data);
    // Draw Texture
    BeginDrawing();
    BeginMode2D(cm.camera);
    DrawTexture(texture, 0, 0, WHITE);
    EndMode2D();
    EndDrawing();
  }

  // Clean up
  UnloadTexture(texture);
  CloseWindow();
  delete[] data;
  return 0;
}