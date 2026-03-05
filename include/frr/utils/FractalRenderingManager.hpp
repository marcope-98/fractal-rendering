#include "FractalRenderer.hpp"
#include "InputHandler.hpp"
#include "Window.hpp"
#include "frr/utils/Camera.hpp"

#include <string>

namespace frr
{
  struct FractalRenderingManager
  {
  public:
    explicit FractalRenderingManager(const std::string &shader_path) : renderer{}, window{shader_path} {}
    auto run() -> void;

  private:
    auto apply(const FrameInput &input) -> void;
    
    FractalRenderer renderer;
    Window          window;
    InputHandler    input;
    Camera          camera;

    static constexpr const char *method_names[]{"Naive", "AVX2", "Threads", "ThreadPool"};
  };
} // namespace frr