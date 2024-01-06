#include "Logger.h"

#include <iostream>

void Logger::LogInfo(const char* message, const char* context)
{
    std::cout << "[INFO][" << context << "] " << std::endl << "\t" << message << std::endl;
}

void Logger::LogWarning(const char* message, const char* context)
{
    std::cout << "[WARNING][" << context << "] " << std::endl << "\t" << message << std::endl;
}

void Logger::LogError(const char* message, const char* context)
{
    std::cout << "[ERROR][" << context << "] " << std::endl << "\t" << message << std::endl;
}

void Test()
{
    
}
