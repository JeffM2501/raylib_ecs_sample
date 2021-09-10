/**********************************************************************************************
*
*   raylib_ECS_sample * a sample Entity Component System using raylib
*
*   LICENSE: ZLIB
*
*   Copyright (c) 2021 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#pragma once

#include "entity.h"

#include <functional>

class Component
{
public:
    uint64_t EntityId;
    bool Active = true;

public:
    Component(uint64_t id) : EntityId(id) {}

    virtual ~Component() = default;
    virtual size_t Id() { return 0; }
    virtual const char* ComponentName() { return nullptr; }

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnUpdate() {}

    inline bool WantUpdate() { return NeedUpdate; }

    template<class T>
    inline T* GetComponent()
    {
        return ComponentManager::GetComponent<T>(this);
    }

    template<class T>
    inline T* MustGetComponent()
    {
        return ComponentManager::MustGetComponent<T>(this);
    }

protected:
    bool NeedUpdate = false;
};

using ComponentObserver = std::function<void(Component*)>;

#define DEFINE_COMPONENT(TYPE) \
    TYPE(uint64_t id) : Component(id) {} \
    static size_t GetComponentId() { return reinterpret_cast<size_t>(#TYPE); } \
    size_t Id() override { return TYPE::GetComponentId(); } \
    const char* ComponentName() override { return #TYPE; } 


#define DEFINE_DERIVED_COMPONENT(TYPE, BASETYPE) \
    TYPE(uint64_t id) : BASETYPE(id) {} \
    static size_t GetComponentId() { return reinterpret_cast<size_t>(#BASETYPE); } \
    size_t Id() override { return TYPE::GetComponentId(); } \
    const char* ComponentName() override { return #TYPE; } 

namespace ComponentManager
{
    Component* StoreComponent(size_t componentId, Component* component);
    void EraseAllComponents(size_t componentId, uint64_t entityId);
    void EraseComponent(size_t componentId, Component* component);
    Component* FindComponent(size_t componentId, uint64_t entityId);
    const std::vector<Component*>& FindComponents(size_t componentId, uint64_t entityId);

    void AddAddObserver(size_t componentId, ComponentObserver observer);
    void AddRemoveObserver(size_t componentId, ComponentObserver observer);

    void RemoveEntity(uint64_t entityId);

    void Update();

    /// <summary>
    /// Iterate all the entities with a component
    /// </summary>
    /// <param name="componentId">The component ID</param>
    /// <param name="func">Callback to be run for every entity with the component</param>
    void DoForEachEntity(size_t componentId, std::function<void(Component*)> func);

    /// <summary>
    /// Iterate all components for an entity
    /// </summary>
    /// <param name="entityId">The entity to itterate</param>
    /// <param name="func">the callback to run for every component on an entity</param>
    void DoForEachComponentInEntity(uint64_t entityId, std::function<void(Component*)> func);

    template<class T>
    inline T* AddComponent(uint64_t entityId)
    {
        T* component = new T(entityId);
        return static_cast<T*>(StoreComponent(component->Id(), component));
    }

    template<class T>
    inline T* AddComponent()
    {
        T* component = new T(EntityManger::CreateEntity());
        return static_cast<T*>(StoreComponent(component->Id(), component));
    }

    template<class T>
    inline T* AddComponent(Component* component)
    {
        if (component == nullptr)
            return AddComponent<T>();

        T* newComponent = new T(component->EntityId);

        return static_cast<T*>(StoreComponent(newComponent->Id(), newComponent));
    }

    template<class T>
    inline void RemoveComponents(uint64_t entityId)
    {
        EraseAllComponents(T::GetComponentId(), entityId);
    }

    template<class T>
    inline void RemoveComponent(Component* component)
    {
        if (component == nullptr)
            return;

        EraseComponent(component->Id(), component);
    }

    template<class T>
    inline T* GetComponent(uint64_t entityId)
    {
        return static_cast<T*>(FindComponent(T::GetComponentId(), entityId));
    }

    template<class T>
    inline T* GetComponent(Component* component)
    {
        return static_cast<T*>(FindComponent(T::GetComponentId(), component->EntityId));
    }

    template<class T>
    inline T* MustGetComponent(uint64_t entityId)
    {
        T* newComponent = static_cast<T*>(FindComponent(T::GetComponentId(), entityId));
        if (newComponent != nullptr)
            return newComponent;

        return AddComponent<T>(entityId);
    }

    template<class T>
    inline T* MustGetComponent(Component* component)
    {
        T* newComponent = static_cast<T*>(FindComponent(T::GetComponentId(), component->EntityId));
        if (newComponent != nullptr)
            return newComponent;

        return AddComponent<T>(component->EntityId);
    }

    template<class T>
    inline void AddAddObserver(ComponentObserver observer)
    {
        AddAddObserver(T::GetComponentId(), observer);
    }

    template<class T>
    inline void AddRemoveObserver(ComponentObserver observer)
    {
        AddRemoveObserver(T::GetComponentId(), observer);
    }

    template<class T>
    inline void AddAddbs(Component* component)
    {
        T* newComponent = static_cast<T*>(FindComponent(T::GetComponentId(), component->EntityId));
        if (newComponent != nullptr)
            return newComponent;

        return AddComponent<T>(component->EntityId);
    }

    /// <summary>
    /// Iterate all the entities with a component
    /// </summary>
    /// <typeparam name="T">Component to iterate</typeparam>
    /// <param name="func">callback to call with each entity that has a component</param>
    template<class T>
    inline void DoForEachEntity(std::function<void(T*)> func)
    {
        DoForEachEntity(T::GetComponentId(), [func](Component* comp) {func(static_cast<T*>(comp)); });
    }
}