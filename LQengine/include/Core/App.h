#pragma once
#include "Core/ModuleRegistry.h"
#include "Core/DefaultModules.h"
#include "Window/Window.h"
#include "Utils/Log.h"
#include <optional>

namespace LQ {
    class RendererContext;

    class App{
        public:
            App(int argc, char** argv);
            ~App();

            auto init(this App& self) -> void;
            auto run(this App& self) -> void;
            auto step(this App& self) -> void;
            auto stop(this App& self) -> void;

            auto should_stop(this App& self) -> void;

            auto set_name(this App& self, std::string name) -> App&;
            auto set_window(this App& self, WindowInfo window_info) -> App&;

            static auto get_window() -> const Window&;
            static auto get_rendercontext() -> RendererContext&;

            static App* get() { return m_instance; }
            static void set_instance(App* instance);

            template <typename T, typename... Args>
            auto with(this App& self, Args&&... args) -> App& {
                self.m_registry.add<T>(std::forward<Args>(args)...);

                return self;
            }

            template <typename... Modules>
            auto with(this App& self, std::tuple<Modules...>) -> App& {
                (..., [&] {
                self.with<Modules>();
                }());

                return self;
            }

            template <typename T>
            static auto mod() -> T& {
                return get()->m_registry.get<T>();
            }

            template <typename T>
            static auto has_mod() -> bool {
                return get()->m_registry.has<T>();
            }
        private:
            std::string m_name = "LQ";
            ModuleRegistry m_registry{};

            std::optional<Window>      m_window{std::nullopt};
            std::optional<WindowInfo>  m_window_info{std::nullopt};

            std::unique_ptr<RendererContext> m_renderer_context{};

            bool m_is_running{true};

            static App* m_instance;
    };
}
