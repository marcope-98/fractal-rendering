#include <chrono>
#include <iostream>

#include "frr/utils/FractalRenderingManager.hpp"

int main(int argc, char *argv[])
{
  try
  {
    if (argc != 2)
      throw std::invalid_argument{"Incorrect number of arguments provided. Expected 1, got " + std::to_string(argc - 1)};

    frr::FractalRenderingManager frm{argv[1]};
    frm.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return 0;
}