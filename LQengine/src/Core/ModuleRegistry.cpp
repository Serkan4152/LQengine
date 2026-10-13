#include "Core/ModuleRegistry.h"
#include "Utils/Log.h"

namespace LQ{
    auto ModuleRegistry::init(this ModuleRegistry &self) -> bool
    {
        for(const auto& record : self.module_records) {
            auto result = record.init_callback();
            if(!result.has_value()) {
                LQ_LOG_ERROR("Module {} failed to initialize: {}", record.name, result.error());
                return false;
            }

            LQ_LOG_INFO("Module {} initialized successfully.", record.name);
        }
        return true;
    }

    auto ModuleRegistry::deinit(this ModuleRegistry &self) -> bool
    {
        for(const auto& record : self.module_records) {
            auto result = record.deinit_callback();
            if(!result.has_value()) {
                LQ_LOG_ERROR("Module {} failed to deinitialize: {}", record.name, result.error());
                return false;
            }

            self.registry.erase(record.type);
            LQ_LOG_INFO("Module {} deinitialized successfully.", record.name);
        }
        return true;
    }

    auto ModuleRegistry::update(this ModuleRegistry &self) -> void
    {
        for(const auto& record : self.module_records) {
            if(record.update_callback) {
                record.update_callback();
            }
        }
    }
}
