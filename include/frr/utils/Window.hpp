#ifndef FRR_WINDOW_HPP_
#define FRR_WINDOW_HPP_
#include <cstddef>
#include <cstdint>
#include <string>

#include "raylib.h"

#include "frr/common.hpp"

namespace frr
{
  struct Window
  {
    explicit Window(const std::string &shader_path);
    ~Window();

    Window(const Window &) = delete;
    Window(Window &&)      = delete;

    auto should_close() const -> bool { return WindowShouldClose(); }
    auto upload(const std::uint32_t *data) -> void { UpdateTexture(this->texture, data); }
    auto draw(std::size_t max_iterations, const std::string &duration, const std::string &method_name) -> void;

  private:
    Texture2D texture;
    Shader    shader;
    int       max_iterations_loc;
  };
} // namespace frr

#endif // FRR_WINDOW_HPP_