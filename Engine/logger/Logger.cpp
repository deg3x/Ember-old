#include "Logger.h"

#include <iostream>

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
        
        std::cout << categoryMsg << "[" << entry.context << "]\n\t" << entry.message;
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

void Logger::Log(LogCategory category, const std::string& context, const std::string& message, LogOutput output)
{
    LogEntry newEntry;

    newEntry.category = category;
    newEntry.context  = context;
    newEntry.message  = message;

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
