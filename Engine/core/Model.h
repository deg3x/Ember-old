#pragma once

#include <vector>
#include <memory>

class Mesh;

class Model
{
public:
    Model() = delete;
    
    static std::vector<std::shared_ptr<Mesh>> Load(const char* path);
};