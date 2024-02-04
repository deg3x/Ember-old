#pragma once

#include "Definitions.h"

class Object;
class Texture;

class ENGINE_API ObjectPrimitive
{
public:
    ObjectPrimitive() = delete;

    static std::shared_ptr<Object> InstantiateCube();
    static std::shared_ptr<Object> InstantiatePlane();
    static std::shared_ptr<Object> InstantiateSphere();
    static std::shared_ptr<Object> InstantiateSkybox();
    static std::shared_ptr<Object> InstantiateEditorGrid();
};
