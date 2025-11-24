#ifndef FRACTALRENDERINGMANAGER_HPP_
#define FRACTALRENDERINGMANAGER_HPP_

#include <cstdint>
#include <filesystem>
#include <iostream>
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
  uint32_t       *data{nullptr};
  std::size_t     max_iterations{64};
  frr::ThreadPool tp;

  // raylib
  Texture2D   texture;
  std::string methods[4] = {"Naive", "AVX2", "Threads", "ThreadPool"};
  Shader      shader;

  // user input
  frr::Camera cm;
  int         last_key_pressed{0};

  FractalRenderingManager(const std::string &shader_location)
  {
    if (!std::filesystem::exists(std::filesystem::path{shader_location}))
    {
      std::cerr << "Specified shader file " << shader_location << " does not exist. Please check your path.\n";
      exit(1);
    }

    this->data = new std::uint32_t[frr::width * frr::height]();
    this->tp.init(this->data);

    InitWindow(frr::width, frr::height, "Fractal Rendering");
    Image image = {
        this->data,                       /* data */
        frr::width,                       /* width */
        frr::height,                      /* height */
        1,                                /* mipmaps */
        PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 /* format */
    };
    this->texture = LoadTextureFromImage(image);
    this->shader  = LoadShader(0, shader_location.c_str());
  }

  ~FractalRenderingManager()
  {
    UnloadShader(this->shader);
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
        frr::naive(this->data, this->cm.getTL(), this->cm.delta(), this->max_iterations);
        break;
      case 1:
        frr::simd(this->data, this->cm.getTL(), this->cm.delta(), this->max_iterations);
        break;
      case 2:
        frr::threads(this->data, this->cm.getTL(), this->cm.delta(), this->max_iterations);
        break;
      case 3:
        tp.run(this->cm.getTL(), this->cm.delta(), this->max_iterations);
        break;
    }
  }

  void render(const std::string &duration)
  {
    UpdateTexture(this->texture, this->data);
    SetShaderValue(this->shader, GetShaderLocation(this->shader, "max_iterations"), &this->max_iterations, SHADER_UNIFORM_INT);
    // clang-format off
    BeginDrawing();
      BeginShaderMode(this->shader);
        DrawTexture(this->texture, 0, 0, WHITE);
      EndShaderMode();
      DrawText((duration + " [ms] | Max Iterations: " + std::to_string(this->max_iterations) + 
                                " | " + this->methods[this->last_key_pressed]).c_str(),
                0, 0, 20, RAYWHITE);
    EndDrawing();
    // clang-format on
  }
};

#endif // FRACTALRENDERINGMANAGER_HPP_