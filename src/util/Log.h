//
// Created by quartzy on 2/7/21.
//

#ifndef MCCPP_LOG_H
#define MCCPP_LOG_H

#include "spdlog/spdlog.h"

namespace MCCPP{

class Log {
public:
    static void init();

    inline static std::shared_ptr<spdlog::logger>& getCoreLogger() {return coreLogger;}
    inline static std::shared_ptr<spdlog::logger>& getClientLogger() {return clientLogger;}

    static std::shared_ptr<spdlog::logger> clientLogger;
    static std::shared_ptr<spdlog::logger> coreLogger;
};

}

#ifdef NDEBUG

#define CORE_FATAL(...)
#define CORE_ERROR(...)
#define CORE_WARN(...)
#define CORE_INFO(...)
#define CORE_TRACE(...)

#define CLIENT_FATAL(...)
#define CLIENT_ERROR(...)
#define CLIENT_WARN(...)
#define CLIENT_INFO(...)
#define CLIENT_TRACE(...)

#else

#define CORE_FATAL(...) MCCPP::Log::getCoreLogger()->fatal(__VA_ARGS__)
#define CORE_ERROR(...) MCCPP::Log::getCoreLogger()->error(__VA_ARGS__)
#define CORE_WARN(...) MCCPP::Log::getCoreLogger()->warn(__VA_ARGS__)
#define CORE_INFO(...) MCCPP::Log::getCoreLogger()->info(__VA_ARGS__)
#define CORE_TRACE(...) MCCPP::Log::getCoreLogger()->trace(__VA_ARGS__)

#define CLIENT_FATAL(...) MCCPP::Log::getClientLogger()->fatal(__VA_ARGS__)
#define CLIENT_ERROR(...) MCCPP::Log::getClientLogger()->error(__VA_ARGS__)
#define CLIENT_WARN(...) MCCPP::Log::getClientLogger()->warn(__VA_ARGS__)
#define CLIENT_INFO(...) MCCPP::Log::getClientLogger()->info(__VA_ARGS__)
#define CLIENT_TRACE(...) MCCPP::Log::getClientLogger()->trace(__VA_ARGS__)

#endif

#endif //MCCPP_LOG_H
