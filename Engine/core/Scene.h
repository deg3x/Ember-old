#pragma once

#include "../Definitions.h"

#include <vector>
#include <memory>
#include <unordered_set>

class Object;
class Light;
class Camera;

class ENGINE_API Scene
{
public:
    static Scene* Active;
    
private:
    // Change the data structure in the future to better represent a hierarchy?
    std::unordered_set<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    // Only a single scene camera is supported for now
    std::shared_ptr<Camera> camera;
    
public:
    Scene() = default;
    virtual ~Scene();

    void Use();
    void Tick() const;
    
    void AddObject(const std::shared_ptr<Object>& object);
    void RemoveObject(const std::shared_ptr<Object>& object);
};
