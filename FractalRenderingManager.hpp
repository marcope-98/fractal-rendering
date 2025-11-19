#ifndef FRACTALRENDERINGMANAGER_HPP_
#define FRACTALRENDERINGMANAGER_HPP_

#include <cstdint>
#include <string>

#include "raylib.h"

#include "frr/common.hpp"
#include "frr/impl/naive.hpp"
#include "frr/impl/simd.hpp"
#include "frr/impl/threadpool.hpp"
#include "frr/impl/threads.hpp"
#include "frr/utils/Camera.hpp"

struct FractalRenderingManager
{
  // fractal rendering
  uint8_t *       data{nullptr};
  std::size_t     max_iterations{64};
  frr::ThreadPool tp;

  // raylib
  Texture2D   texture;
  std::string methods[4] = {"Naive", "AVX2", "Threads", "ThreadPool"};

  // user input
  frr::Camera cm;
  int         last_key_pressed{0};

  FractalRenderingManager()
  {
    this->data = new std::uint8_t[frr::width * frr::height]();
    this->tp.init(this->data);

    InitWindow(frr::width, frr::height, "Fractal Rendering");
    Image image = {
        this->data,                         /* data */
        frr::width,                         /* width */
        frr::height,                        /* height */
        PIXELFORMAT_UNCOMPRESSED_GRAYSCALE, /* format */
        1                                   /* mipmaps */
    };
    this->texture = LoadTextureFromImage(image);
  }

  ~FractalRenderingManager()
  {
    UnloadTexture(this->texture);
    CloseWindow();
    this->tp.shutdown();
    delete[] this->data;
  }

  void user_input()
  {
    // Camera controls
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
      this->cm.reset();
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

    // Keypad controls
    if (IsKeyPressed(KEY_KP_ADD)) this->max_iterations += 32;
    if (IsKeyPressed(KEY_KP_SUBTRACT)) this->max_iterations = this->max_iterations == 32 ? 32 : this->max_iterations - 32;

    // Method selection
    if (int key = GetKeyPressed(); KEY_ONE <= key && key <= KEY_FOUR) this->last_key_pressed = key - KEY_ONE;
  }

  void run()
  {
    switch (this->last_key_pressed)
    {
      case 0:
        frr::naive(this->data, this->cm.getTL(), this->cm.getBR(), this->max_iterations);
        break;
      case 1:
        frr::simd(this->data, this->cm.getTL(), this->cm.getBR(), this->max_iterations);
        break;
      case 2:
        frr::threads(this->data, this->cm.getTL(), this->cm.getBR(), this->max_iterations);
        break;
      case 3:
        tp.run(this->cm.getTL(), this->cm.getBR(), this->max_iterations);
        break;
    }
  }

  void render(const std::string &duration)
  {
    UpdateTexture(this->texture, this->data);
    // clang-format off
    BeginDrawing();
      DrawTexture(this->texture, 0, 0, WHITE);
      DrawText((duration + " [ms] | Max Iterations: " + std::to_string(this->max_iterations) + 
                                " | " + this->methods[this->last_key_pressed]).c_str(),
                0, 0, 20, RAYWHITE);
    EndDrawing();
    // clang-format on
  }
};

#endif // FRACTALRENDERINGMANAGER_HPP_