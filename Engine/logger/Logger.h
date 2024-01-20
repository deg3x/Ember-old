#pragma once

#include <vector>

#include "Definitions.h"
#include "LoggerTypes.h"

class ENGINE_API Logger
{
public:
    static std::vector<LogEntry> consoleLog;
    
public:
    static void Log(const LogEntry& entry, LogOutput output = (LogOutput)(LogOutput::STDOUT | LogOutput::CONSOLE));
    static void Log(LogCategory category, const std::string& context, const std::string& message, LogOutput output = (LogOutput)(LogOutput::STDOUT | LogOutput::CONSOLE));

    static void ClearConsoleLog();
    static std::vector<LogEntry> GetConsoleLog();
};
