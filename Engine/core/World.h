#pragma once

#include "Definitions.h"

#include <vector>
#include <memory>
#include <unordered_set>

class Object;
class Light;
class Camera;

enum class ObjectType
{
    OPAQUE,
    TRANSPARENT
};

class ENGINE_API World
{
private:
    // Change the data structure in the future to better represent a hierarchy?
    static std::unordered_set<std::shared_ptr<Object>> objQueueOpaque;
    static std::unordered_set<std::shared_ptr<Object>> objQueueTransparent;
    static std::vector<std::shared_ptr<Light>> lights;

    // Only a single scene camera is supported for now
    static std::shared_ptr<Camera> camera;
    
public:
    World() = delete;

    static void Initialize();
    static void Tick();
    
    static void AddObject(const std::shared_ptr<Object>& object, ObjectType type);
    static void RemoveObject(const std::shared_ptr<Object>& object);

    static inline std::shared_ptr<Camera> GetCamera()
    {
        return camera;
    }
};
