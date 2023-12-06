#include "engine/window/window.h"

#include <fmt/format.h>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include <iostream>

namespace engine::window {

Window::Window(Params params) : params_(params) {}

int Window::init() {
    /*
      * Initialises the SDL video subsystem (as well as the events subsystem).
      * Returns 0 on success or a negative error code on failure using SDL_GetError().
      */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        return 1;
    }

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window_ = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, params_.window_width,
                               params_.window_height, window_flags);
    gl_context_ = SDL_GL_CreateContext(window_);
    renderer_ = SDL_CreateRenderer(
        window_, -1, static_cast<SDL_RendererFlags>(SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
    SDL_GL_MakeCurrent(window_, gl_context_);
    SDL_GL_SetSwapInterval(1);  // Enable vsync

    //    // Select the color for drawing. It is set to red here.
    //    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
    //    // Clear the entire screen to our selected color.
    //    SDL_RenderClear(renderer_);

    glViewport(0, 0, params_.window_width, params_.window_height);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    imgui_io_ = &ImGui::GetIO();
    imgui_io_->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    imgui_io_->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    //    ImGui_ImplSDL2_InitForOpenGL(window_, gl_context_);

    ImGui_ImplSDL2_InitForSDLRenderer(window_, renderer_);
    ImGui_ImplSDLRenderer2_Init(renderer_);

    return 0;
}

void Window::term() {
    // Cleanup
    //    ImGui_ImplOpenGL3_Shutdown();
    //    ImGui_ImplSDL2_Shutdown();
    //    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer_);
    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

bool Window::new_frame() {
    while (SDL_PollEvent(&sdl_event_)) {
        if (sdl_event_.type == SDL_QUIT) {
            return false;
        }
        ImGui_ImplSDL2_ProcessEvent(&sdl_event_);  // Send events to ImGui
    }

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    static bool ok = true;
    ImGui::ShowDemoWindow(&ok);

    return true;
}

void Window::render() {
    assert(imgui_io_);

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderSetScale(renderer_, imgui_io_->DisplayFramebufferScale.x, imgui_io_->DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(renderer_, (Uint8)(params_.clear_color[0] * 255), (Uint8)(params_.clear_color[1] * 255),
                           (Uint8)(params_.clear_color[2] * 255), (Uint8)(params_.clear_color[3] * 255));
    SDL_RenderPresent(renderer_);
    SDL_RenderClear(renderer_);

    SDL_Delay(1000 / 60);
}

Window::~Window() {
    term();
}

SDL_Renderer* Window::get_renderer() {
    return renderer_;
}

Window::Size Window::get_size() const {
    int w = 0;
    int h = 0;
    SDL_GetWindowSize(window_, &w, &h);
    return {w, h};
}

void Window::set_params(Window::Params params) {
    params_ = params;
}

}  // namespace engine::window
