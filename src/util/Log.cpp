//
// Created by quartzy on 2/7/21.
//

#include <spdlog/sinks/stdout_color_sinks.h>
#include "Log.h"

std::shared_ptr<spdlog::logger> Log::clientLogger;
std::shared_ptr<spdlog::logger> Log::coreLogger;

void SetStdinEcho(bool enable)
{
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if( !enable )
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode );

#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if( !enable )
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

void Log::init() {
    spdlog::set_pattern("%^[%T] %n(%l): %v%$");
    coreLogger = spdlog::stdout_color_mt("Core");
    coreLogger->set_level(spdlog::level::trace);
    clientLogger = spdlog::stdout_color_mt("Client");
    clientLogger->set_level(spdlog::level::trace);
}