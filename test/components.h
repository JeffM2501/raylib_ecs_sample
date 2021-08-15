#pragma once

#include "entity.h"

#include <functional>

class Component
{
public:
    uint64_t EntityId;

public:
    Component(uint64_t id) : EntityId(id) {}

    virtual ~Component() = default;

    virtual void OnCreate() {}
    virtual void OnDestory() {}
    virtual void OnUpdate() {}

    inline bool WantUpdate() { return NeedUpdate; }

protected:
    bool NeedUpdate = false;
};

#define GET_COMP_NAME(TYPE) #TYPE

#define DEFINE_COMPONENT(TYPE) \
    TYPE(uint64_t id) : Component(id) {} \
    static size_t GetComponentId () { return reinterpret_cast<size_t>(#TYPE);}

namespace ComponentManager
{
    Component* StoreComponent(size_t componentId, Component* component);
    void EraseComponent(size_t componentId, uint64_t entityId);
    Component* FindComponent(size_t componentId, uint64_t entityId);
    void RemoveEntity(uint64_t entityId);

    void Update();

    void DoForEachEntity(size_t componentId, std::function<void(Component*)> func);

    template<class T>
    inline T* AddComponent(uint64_t entityId)
    {
        return static_cast<T*>(StoreComponent(T::GetComponentId(), new T(entityId)));
    }

    template<class T>
    inline T* AddComponent()
    {
        return static_cast<T*>(StoreComponent(T::GetComponentId(), new T(EntityManger::CreateEntity())));
    }

    template<class T>
    inline T* AddComponent(Component* component)
    {
        if (component == nullptr)
            return AddComponent<T>(EntityManger::CreateEntity());

        return static_cast<T*>(StoreComponent(T::GetComponentId(), new T(component->EntityId)));
    }

    template<class T, class D>
    inline T* AddComponent(uint64_t entityId)
    {
        return static_cast<T*>(StoreComponent(T::GetComponentId(), new D(entityId)));
    }

    template<class T, class D>
    inline T* AddComponent(Component* component)
    {
        if (component == nullptr)
            return AddComponent<T>(EntityManger::CreateEntity());

        return static_cast<T*>(StoreComponent(T::GetComponentId(), new D(component->EntityId)));
    }

    template<class T>
    inline void RemoveComponent(uint64_t entityId)
    {
        EraseComponent(T::GetComponentId(), entityId);
    }

    template<class T>
    inline void RemoveComponent(Component* component)
    {
        if (component == nullptr)
            return;

        EraseComponent(T::GetComponentId(), component->EntityId);
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
    inline void DoForEachEntity(std::function<void(T*)> func)
    {
        DoForEachEntity(T::GetComponentId(), [func](Component* comp) {func(static_cast<T*>(comp)); });
    }
}
