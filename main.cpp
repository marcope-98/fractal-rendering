#include <iostream>

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
  InitWindow(1280, 720, "Fractal Rendering");
  CameraManager cm;

  while (!WindowShouldClose())
  {
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) cm.reset();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) cm.pan();
    if (float wheel = GetMouseWheelMove(); wheel != 0) cm.zoom(wheel);
    cm.bound();

    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode2D(cm.camera);
    DrawRectangle(0.f, 0.f, GetScreenWidth(), GetScreenHeight(), MAROON);
    DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 50, BLACK);
    EndMode2D();

    DrawCircleV(GetMousePosition(), 4, DARKGRAY);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}