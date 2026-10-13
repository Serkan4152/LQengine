#pragma once
#include <concepts>
#include <string_view>
#include <typeindex>
#include <expected>
#include "ankerl/unordered_dense.h"

#include "Core/Types.h"
#include "Utils/Log.h"

namespace LQ {
    template <typename T>
    concept Module = requires(T t) {
    t.init();
    t.deinit();
    { T::MODULE_NAME } -> std::convertible_to<std::string_view>;
    };

    template <typename T>
    concept ModuleHasUpdate = requires(T t) { t.update(); };

    struct ModuleRegistry {
        using ModulePtr = std::unique_ptr<void, void (*)(void*)>;
        using Registry = ankerl::unordered_dense::map<std::type_index, ModulePtr, TypeIndexHash>;

        struct ModuleRecord{
            std::string_view name{};
            std::type_index  type{typeid(void)};

            std::function<std::expected<void, std::string>()> init_callback{};
            std::function<std::expected<void, std::string>()> deinit_callback{};
            std::function<void()> update_callback {};
        };

        Registry registry{};
        std::vector<ModuleRecord> module_records{};

        auto init(this ModuleRegistry& self) -> bool;
        auto deinit(this ModuleRegistry& self) -> bool;
        auto update(this ModuleRegistry& self) -> void;

        template <Module T, typename... Args>
        auto add(Args&&... args) -> void {
            using DependenciesTuple = typename get_module_dependencies<T>::type;
            check_dependencies<T>(DependenciesTuple{});

            auto type_index = std::type_index(typeid(T));

            auto deleter = [](void* self) {
                delete static_cast<T*>(self);
            };

            auto& module = registry.try_emplace(type_index, ModulePtr(new T(std::forward<Args>(args)...), deleter)).first->second;

            ModuleRecord record{
                .name = T::MODULE_NAME,
                .type = type_index,
                .init_callback = [m = static_cast<T*>(module.get())]() {
                    return m->init();
                },
                .deinit_callback = [m = static_cast<T*>(module.get())]() {
                    return m->deinit();
                }
            };

            if constexpr (ModuleHasUpdate<T>) {
                record.update_callback = [m = static_cast<T*>(module.get())]() {
                    return m->update();
                };
            }

            module_records.emplace_back(std::move(record));
        }

        template <typename T>
        auto has() const -> bool {
            return registry.contains(std::type_index(typeid(T)));
        }

        template <typename T>
        auto get() -> T& {
            auto it = registry.find(std::type_index(typeid(T)));
            if(it == registry.end()) {
                LQ_LOG_ERROR("Module {} not found in registry.", T::MODULE_NAME);
            }

            return *static_cast<T*>(it->second.get());
        }

        template <typename T>
        struct get_module_dependencies {
            using type = std::tuple<>;
        };

        template <typename T>
            requires requires { typename T::module_dependencies; }
        struct get_module_dependencies<T> {
            using type = typename T::module_dependencies;
        };

        template <typename ModuleT, typename... DepTypes>
        void check_dependencies(std::tuple<DepTypes...>){
            bool all_met = true;
            std::string missing_deps_msg;

            (..., [&] {
                if (!has<DepTypes>()) {
                    all_met = false;
                    if (!missing_deps_msg.empty())
                    missing_deps_msg += ", ";
                    missing_deps_msg += std::string(DepTypes::MODULE_NAME);
                }
            }());

            LQ_ASSERT(all_met, "Module " + std::string(ModuleT::MODULE_NAME) + " is missing dependencies: " + missing_deps_msg);
        }
    };
}