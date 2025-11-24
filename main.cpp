#include <chrono>
#include <iostream>

#include "FractalRenderingManager.hpp"
#include "raylib.h"

int main(int argc, char *argv[])
{
  try
  {
    if (argc != 2)
      throw std::invalid_argument{"Incorrect number of arguments provided. Expected 1, got " + std::to_string(argc + 1)};

    FractalRenderingManager frm(argv[1]);
    while (!WindowShouldClose())
    {
      frm.user_input();

      auto begin = std::chrono::steady_clock::now();
      frm.run();
      auto end = std::chrono::steady_clock::now();

      frm.render(std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()));
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return 0;
}