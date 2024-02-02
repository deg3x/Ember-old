#pragma once

#include "Definitions.h"

class Object;
class Light;
class Camera;

class ENGINE_API World
{
    friend class Hierarchy; // This is a dirty quick way to access the world elements from the editor hierarchy. CHANGE IN THE FUTURE
private:
    // Change the data structure in the future to better represent a hierarchy?
    static std::unordered_set<std::shared_ptr<Object>> objQueueOpaque;
    static std::unordered_set<std::shared_ptr<Object>> objQueueTransparent;
    static std::unordered_set<std::shared_ptr<Light>> lights;

    // Only a single scene camera is supported for now
    static std::shared_ptr<Camera> camera;
    
public:
    World() = delete;

    static void Initialize();
    static void Tick();
    
    static void AddObject(const std::shared_ptr<Object>& object);
    static void RemoveObject(const std::shared_ptr<Object>& object);

    static void AddLight(const std::shared_ptr<Light>& light);
    static void RemoveLight(const std::shared_ptr<Light>& light);

    static inline std::shared_ptr<Camera> GetCamera()
    {
        return camera;
    }
};
