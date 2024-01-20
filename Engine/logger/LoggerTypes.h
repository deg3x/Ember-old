#pragma once

#include "Definitions.h"

#include <cstdint>
#include <string>

enum class LogCategory : uint8_t
{
    INFO,
    WARNING,
    ERROR,
};

enum LogOutput : uint8_t
{
    STDOUT  = 0x01,
    CONSOLE = 0x02,
    LOGFILE = 0x04, // We do not support this for now but we have it here for future reference
};

struct LogEntry
{
    std::string message;
    std::string context;
    LogCategory category;
};
