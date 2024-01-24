#pragma once

#include "Definitions.h"

#include <type_traits>
#include <memory>

enum class ComponentType
{
    TRANSFORM,
    CAMERA,
    MESH,
    LIGHT,
    COUNT
};

#define MAKE_UNIQUE_COMPONENT()             \
public:                                     \
    virtual inline bool IsUnique() override \
    {                                       \
        return true;                        \
    }                                       \

#define MAKE_COMPONENT_TYPE(type)           \
public:                                     \
    virtual inline ComponentType GetType()  \
    {                                       \
        return ComponentType::type;         \
    }

class Object;

class ENGINE_API Component
{
    friend class Object;
    
    MAKE_COMPONENT_TYPE(COUNT)
    
protected:
    // Handled by the AddComponent() function of Object
    // Do not modify manually
    Object* owner;

public:
    template <class Type, typename... Args>
    static std::shared_ptr<Type> CreateComponent(Args... args);

    virtual void Tick() {}

    inline Object* GetOwner() const
    {
        return owner;
    }
    
    virtual inline bool IsUnique()
    {
        return true;
    }
};

template <class Type, typename ... Args>
std::shared_ptr<Type> Component::CreateComponent(Args... args)
{
    if (!std::is_base_of<Component, Type>())
    {
        return nullptr;
    }

    return std::make_shared<Type>(args...);
}
