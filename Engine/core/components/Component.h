#pragma once

#include <type_traits>
#include <memory>

#define MAKE_UNIQUE_COMPONENT()             \
    virtual inline bool IsUnique() override \
    {                                       \
        return true;                        \
    }                                       \

class Object;

class Component
{
public:
    // Handled by the AddComponent() function of Object
    // Do not modify manually
    Object* parent;

public:    
    template <class Type>
    static std::shared_ptr<Type> CreateDefaultComponent();

    template <class Type, typename... Args>
    static std::shared_ptr<Type> CreateComponent(Args... args);

    virtual inline bool IsUnique()
    {
        return true;
    }
};

template <class Type>
std::shared_ptr<Type> Component::CreateDefaultComponent()
{
    if (!std::is_base_of<Component, Type>())
    {
        return nullptr;
    }

    return std::make_shared<Type>();
}

template <class Type, typename ... Args>
std::shared_ptr<Type> Component::CreateComponent(Args... args)
{
    if (!std::is_base_of<Component, Type>())
    {
        return nullptr;
    }

    return std::make_shared<Type>(args...);
}
