#include <chrono>

#include "FractalRenderingManager.hpp"
#include "raylib.h"

int main(int argc, char* argv[])
{
  FractalRenderingManager frm;
  if (argc > 1) frm.upload_shader(argv[1]);
  while (!WindowShouldClose())
  {
    frm.user_input();

    auto begin = std::chrono::steady_clock::now();
    frm.run();
    auto end = std::chrono::steady_clock::now();

    frm.render(std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()));
  }

  return 0;
}