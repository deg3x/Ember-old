#pragma once

#include "Component.h"

#include <vector>
#include <memory>
#include <iostream>

class Object
{
private:
    std::vector<std::shared_ptr<Component>> components;
    
public:
    Object();
    virtual ~Object() = default;

    void Draw() const;

    template <class Type>
    bool CreateComponent();

    template <class Type, typename... Args>
    bool CreateComponent(Args... args);

    template <class Type>
    bool HasComponent() const;

    template <class Type>
    std::shared_ptr<Type> GetComponent() const;

    template <class Type>
    std::vector<std::shared_ptr<Type>> GetComponents() const;

    template <class Type>
    void AddComponent(const std::shared_ptr<Type>& component);

    inline std::vector<std::shared_ptr<Component>> GetComponents() const
    {
        return components;
    }
};

template <class Type>
void Object::AddComponent(const std::shared_ptr<Type>& component)
{
    if (!std::is_base_of<Component, Type>())
    {
        return;
    }
    
    std::shared_ptr<Component> comp = std::dynamic_pointer_cast<Component>(GetComponent<Type>());
    if (comp != nullptr && comp->IsUnique())
    {
        return;
    }
    
    components.push_back(component);
}

template <class Type>
bool Object::CreateComponent()
{
    std::shared_ptr<Component> comp = std::dynamic_pointer_cast<Component>(GetComponent<Type>());
    if (comp != nullptr && comp->IsUnique())
    {
        return false;
    }
    
    std::shared_ptr<Type> component = Component::CreateDefaultComponent<Type>();
    if (component == nullptr)
    {
        return false;
    }

    AddComponent(component);
    return true;
}

template <class Type, typename... Args>
bool Object::CreateComponent(Args... args)
{
    std::shared_ptr<Component> comp = std::dynamic_pointer_cast<Component>(GetComponent<Type>());
    if (comp != nullptr && comp->IsUnique())
    {
        return false;
    }
    
    std::shared_ptr<Type> component = Component::CreateComponent<Type>(args...);
    if (component == nullptr)
    {
        return false;
    }

    AddComponent(component);
    return true;
}

template <class Type>
bool Object::HasComponent() const
{
    if (!std::is_base_of<Component, Type>())
    {
        return false;
    }
    
    for (const std::shared_ptr<Component>& component : components)
    {
        if (dynamic_cast<Type*>(component.get()) == nullptr)
        {
            continue;
        }

        return true;
    }

    return false;
}

template <class Type>
std::shared_ptr<Type> Object::GetComponent() const
{
    if (!std::is_base_of<Component, Type>())
    {
        return nullptr;
    }

    for (const std::shared_ptr<Component>& component : components)
    {
        if (dynamic_cast<Type*>(component.get()) == nullptr)
        {
            continue;
        }

        return std::dynamic_pointer_cast<Type>(component);
    }

    return nullptr;
}

template <class Type>
std::vector<std::shared_ptr<Type>> Object::GetComponents() const
{
    std::vector<std::shared_ptr<Type>> comps;

    if (!std::is_base_of<Component, Type>())
    {
        return comps;
    }

    for (const std::shared_ptr<Component>& component : components)
    {
        if (dynamic_cast<Type*>(component.get()) == nullptr)
        {
            continue;
        }

        comps.push_back(std::dynamic_pointer_cast<Type>(component));
    }

    return comps;
}
