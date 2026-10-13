#include "Core/App.h"
#include "Renderer/RendererContext.h"

#include "SDL3/SDL.h"

namespace LQ {
  App* App::m_instance = nullptr;

  App::App(int argc, char** argv) {
    if (m_instance) {
      LQ_LOG_ERROR("Application already exists.");
      return;
    }

    m_instance = this;

    Log::init();
  }

  App::~App() {
    
  }

  auto App::init(this App &self) -> void
  {
    LQ_ASSERT(SDL_Init(SDL_INIT_VIDEO), "Failed to Initialize SDL, Error Code:{}", SDL_GetError());
    
    if(self.m_window_info) {
      self.m_window = Window::create(self.m_window_info.value());
    }

    if (self.m_registry.has<Renderer>()) {
      self.m_renderer_context = std::make_unique<RendererContext>();
      self.m_renderer_context->init();
    }

    LQ_ASSERT(self.m_registry.init(), "Failed to initialize modules.");
  }

  auto App::run(this App &self) -> void
  {
    self.init();

    while(self.m_is_running) {
      self.step();
    }

    self.stop();
  }

  auto App::step(this App &self) -> void
  {
    if (self.m_window)
      self.m_window->update();

    self.m_registry.update();
  }

  auto App::stop(this App &self) -> void
  {
    if (self.m_window)
      self.m_window->destroy();

    SDL_Quit();

    self.m_registry.deinit();
  }

  auto App::should_stop(this App &self) -> void
  {
    self.m_is_running = false;
  }

  auto App::set_name(this App &self, std::string name) -> App &
  {
    self.m_name = name;
    return self;
  }

  auto App::set_window(this App &self, WindowInfo window_info) -> App &
  {
    self.m_window_info = window_info;
    return self;
  }

  auto App::get_window() -> const Window &
  {
    LQ_ASSERT(m_instance->m_window, "Window is not initialized.");
    return m_instance->m_window.value();
  }

  auto App::get_rendercontext() -> RendererContext &
  {
    return *m_instance->m_renderer_context;
  }

  void App::set_instance(App* instance)
  {
    m_instance = instance;
  }
}