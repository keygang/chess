#pragma once

#include "engine/base/color.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui.h>

namespace engine::window {

class Window {
public:
    struct Params {
        int window_width = 1280;
        int window_height = 720;
        base::Color clear_color = base::Color::kWhite;
    };

public:
    explicit Window(Params params);
    ~Window();

    void set_params(Params params);

    int init();
    void term();

    bool new_frame();
    void render();

    struct Size {
        int width = 0;
        int height = 0;
    };

    Size get_size() const;

    SDL_Renderer* get_renderer();

private:
    Params params_;

    SDL_Window* window_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;
    SDL_Event sdl_event_{};
    SDL_Renderer* renderer_ = nullptr;

    ImGuiIO* imgui_io_ = nullptr;
};

}  // namespace engine::window