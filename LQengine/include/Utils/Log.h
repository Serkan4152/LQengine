#pragma once
#include <cstdlib>
#include <memory>
#include <vector>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#define LQ_LOG_INFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__)
#define LQ_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__)
#define LQ_LOG_WARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__)
#define LQ_LOG_ERROR(...)  SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__)
#define LQ_LOG_FATAL(...)  SPDLOG_LOGGER_CRITICAL(spdlog::default_logger_raw(), __VA_ARGS__)

#define LQ_ASSERT(x, ...)                                        \
    do {                                                         \
        if (!(x)) {                                              \
            LQ_LOG_FATAL("Assertion failed: " __VA_ARGS__);      \
            std::abort();                                        \
        }                                                        \
    } while (0)

namespace LQ{
    class Log{
        public:
            static void init(bool saveLog = true) {
                std::vector<spdlog::sink_ptr> sinks;
                sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

                if(saveLog) {
                    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                        "log.txt",
                        true
                    ));
                }

                auto logger = std::make_shared<spdlog::logger>(
                    "LQ",
                    sinks.begin(),
                    sinks.end()
                );
                
                logger->flush_on(spdlog::level::err);
                logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%s:%#] [%l] %v");

                auto bgfx_logger = std::make_shared<spdlog::logger>("BGFX", sinks.begin(), sinks.end()); 

                bgfx_logger->flush_on(spdlog::level::err); 
                bgfx_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%s:%#] [%l] %v");

                spdlog::register_logger(logger);
                spdlog::register_logger(bgfx_logger);

                spdlog::set_default_logger(logger);

                #if defined(_DEBUG)
                    spdlog::set_level(spdlog::level::debug);
                #endif
            }

            static auto get_bgfx_logger() -> std::shared_ptr<spdlog::logger> { 
                return spdlog::get("BGFX"); 
            }
    };
}