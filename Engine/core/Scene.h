#pragma once

#include <vector>
#include <memory>

class Object;

class Scene
{
private:
    // Change the data structure in the future to better represent a hierarchy?
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Object>> lights;

    // Only a single scene camera is supported for now
    std::shared_ptr<Object> camera;
    
public:
    Scene() = default;
    virtual ~Scene() = default;

    void Tick() const;
    void AddObject(const std::shared_ptr<Object>& object);

private:
    void Draw() const;
};
