#include <chrono>

#include "FractalRenderingManager.hpp"
#include "raylib.h"

int main(void)
{
  FractalRenderingManager frm;
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