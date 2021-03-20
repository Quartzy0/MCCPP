//
// Created by quartzy on 2/7/21.
//

#include <spdlog/sinks/stdout_color_sinks.h>
#include "Log.h"
namespace MCCPP {

    std::shared_ptr<spdlog::logger> Log::clientLogger;
    std::shared_ptr<spdlog::logger> Log::coreLogger;

    void Log::init() {
        spdlog::set_pattern("%^[%T] %n(%l): %v%$");
        coreLogger = spdlog::stdout_color_mt("Core");
        coreLogger->set_level(spdlog::level::trace);
        clientLogger = spdlog::stdout_color_mt("Client");
        clientLogger->set_level(spdlog::level::trace);
    }
}