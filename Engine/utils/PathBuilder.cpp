#include "PathBuilder.h"


std::string PathBuilder::GetPath(const std::string& path)
{
#if defined(_WIN32)
    return path;
#elif __APPLE__
    return "../../" + path;
#endif
}
