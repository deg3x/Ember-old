#include "Logger.h"

#include <iostream>

#include "core/Time.h"

std::vector<LogEntry> Logger::consoleLog;

void Logger::Log(const LogEntry& entry, LogOutput output)
{
    if (output & LogOutput::STDOUT)
    {
        std::string categoryMsg;

        switch (entry.category)
        {
        case LogCategory::INFO:
            categoryMsg = "[i]";
            break;
        case LogCategory::WARNING:
            categoryMsg = "[?]";
            break;
        case LogCategory::ERROR:
            categoryMsg = "[!]";
            break;
        default:
            categoryMsg = "[UNKNOWN]";
            break;
        }
        
        std::cout << "[" << Time::TimeSinceStartup << "]" << categoryMsg << "[" << entry.context << "]\n\t" << entry.message << std::endl;
    }

    if (output & LogOutput::CONSOLE)
    {
        consoleLog.emplace_back(entry);
    }

    if (output & LogOutput::LOGFILE)
    {
        // Write to file
    }
}

void Logger::Log(const std::string& message, LogOutput output)
{
    LogEntry newEntry;

    newEntry.category  = LogCategory::INFO;
    newEntry.context   = "NO CONTEXT";
    newEntry.message   = message;
    newEntry.timestamp = Time::TimeSinceStartup;

    Log(newEntry);
}

void Logger::Log(LogCategory category, const std::string& message, const std::string& context, LogOutput output)
{
    LogEntry newEntry;

    newEntry.category = category;
    newEntry.context  = context;
    newEntry.message  = message;
    newEntry.timestamp = Time::TimeSinceStartup;

    Log(newEntry);
}

void Logger::ClearConsoleLog()
{
    consoleLog.clear();
}

std::vector<LogEntry> Logger::GetConsoleLog()
{
    return consoleLog;
}
