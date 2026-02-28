#include "frr/utils/Window.hpp"

#include <filesystem>

frr::Window::Window(const std::string &shader_path)
{
  if (!std::filesystem::is_regular_file(shader_path))
    throw std::filesystem::filesystem_error("The specified shader file does not exist: ", shader_path, std::error_code());

  InitWindow(frr::width, frr::height, "Fractal Rendering");

  Image image              = {nullptr, frr::width, frr::height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
  this->texture            = LoadTextureFromImage(image);
  this->shader             = LoadShader(0, shader_path.c_str());
  this->max_iterations_loc = GetShaderLocation(this->shader, "max_iterations");
}

frr::Window::~Window()
{
  UnloadShader(this->shader);
  UnloadTexture(this->texture);
  CloseWindow();
}

auto frr::Window::draw(std::size_t max_iterations, const std::string &duration, const std::string &method_name) -> void
{
  int mi = static_cast<int>(max_iterations);
  SetShaderValue(this->shader, this->max_iterations_loc, &mi, SHADER_UNIFORM_INT);

  // clang-format off
    BeginDrawing();
    BeginShaderMode(this->shader);
        DrawTexture(this->texture, 0, 0, WHITE);
    EndShaderMode();
    DrawText((duration + " [ms] | Max Iterations: " + std::to_string(max_iterations) + 
                                " | " + method_name).c_str(),
                0, 0, 20, RAYWHITE);
    EndDrawing();
  // clang-format on
}
