#include <chrono>
#include <iostream>

#include "FractalRenderingManager.hpp"
#include "raylib.h"

int main(int argc, char* argv[])
{
  if (argc != 2) 
  {
    std::cerr << "Incorrect number of arguments provided. Expected 1, got " << argc - 1 << "\n";
    return 1;
  }    

  FractalRenderingManager frm(argv[1]);
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