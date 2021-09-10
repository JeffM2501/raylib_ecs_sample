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

#include "components.h"

#include <map>
#include <vector>
#include <algorithm>

using ComponentList = std::vector<Component*>;

class ComponentTable
{
public:
    std::map<uint64_t, ComponentList> Entities;

    std::vector<ComponentObserver> AddObservers;
    std::vector<ComponentObserver> DeleteObservers;

    bool Add(Component* component)
    {
        ComponentList& components = Entities[component->EntityId];
        if (components.empty())
        {
            components.push_back(component);
            return true;
        }

        auto itr = std::find(components.begin(), components.end(), component);
        if (itr != components.end())
            return false;

        components.push_back(component);
        return true;
    }
};

namespace ComponentManager
{
    std::map<size_t, ComponentTable> ComponentDB;

    std::vector<Component*> ComponentUpdateCache;

    void AddAddObserver(size_t componentId, ComponentObserver observer)
    {
        ComponentTable& componentTable = ComponentDB[componentId];
        componentTable.AddObservers.push_back(observer);
    }

    void AddRemoveObserver(size_t componentId, ComponentObserver observer)
    {
        ComponentTable& componentTable = ComponentDB[componentId];
        componentTable.DeleteObservers.push_back(observer);
    }

    Component* StoreComponent(size_t compId, Component* component)
    {
        ComponentTable& componentTable = ComponentDB[compId];

        if (!componentTable.Add(component))
            return component;

        component->OnCreate();

        for (ComponentObserver& observer : componentTable.AddObservers)
            observer(component);

        if (component->WantUpdate())
            ComponentUpdateCache.push_back(component);

        return component;
    }

    Component* FindComponent(size_t compId, uint64_t entityId)
    {
        auto componentTableItr = ComponentDB.find(compId);
        if (componentTableItr == ComponentDB.end())
            return nullptr;

        ComponentTable& componentTable = componentTableItr->second;

        auto entityCacheItr = componentTable.Entities.find(entityId);
        if (entityCacheItr == componentTable.Entities.end() || entityCacheItr->second.empty())
            return nullptr;

        return entityCacheItr->second[0];
    }

    static std::vector<Component*> EmptyComponentList;

    const std::vector<Component*>& FindComponents(size_t compId, uint64_t entityId)
    {
        auto componentTableItr = ComponentDB.find(compId);
        if (componentTableItr == ComponentDB.end())
            return EmptyComponentList;

        ComponentTable& componentTable = componentTableItr->second;

        auto entityCacheItr = componentTable.Entities.find(entityId);
        if (entityCacheItr == componentTable.Entities.end())
            return EmptyComponentList;

        return entityCacheItr->second;
    }

    void EraseAllComponents(size_t compId, uint64_t entityId)
    {
        auto componentTableItr = ComponentDB.find(compId);
        if (componentTableItr == ComponentDB.end())
            return;

        ComponentTable& componentTable = componentTableItr->second;

        auto entityCacheItr = componentTable.Entities.find(entityId);
        if (entityCacheItr != componentTable.Entities.end())
        {
            ComponentList& components = entityCacheItr->second;
            for (Component* component : components)
            {
                component->OnDestroy();
                if (component->WantUpdate())
                    ComponentUpdateCache.erase(std::find(ComponentUpdateCache.begin(), ComponentUpdateCache.end(), component));

                for (ComponentObserver& observer : componentTable.DeleteObservers)
                    observer(component);

                delete(component);
            }

            componentTable.Entities.erase(entityCacheItr);
        }
    }

    void EraseComponent(size_t compId, Component* component)
    {
        auto componentTableItr = ComponentDB.find(compId);
        if (componentTableItr == ComponentDB.end())
            return;

        ComponentTable& componentTable = componentTableItr->second;

        auto entityCacheItr = componentTable.Entities.find(component->EntityId);
        if (entityCacheItr != componentTable.Entities.end())
        {
            ComponentList& components = entityCacheItr->second;

            ComponentList::iterator itr = std::find(components.begin(), components.end(), component);

            component->OnDestroy();

            if (component->WantUpdate())
                ComponentUpdateCache.erase(std::find(ComponentUpdateCache.begin(), ComponentUpdateCache.end(), component));

            for (ComponentObserver& observer : componentTable.DeleteObservers)
                observer(component);

            delete(component);
            components.erase(itr);
        }
    }

    void RemoveEntity(uint64_t entityId)
    {
        for (auto componentTable : ComponentDB)
        {
            EraseAllComponents(componentTable.first, entityId);
        }
    }

    void Update()
    {
        for (auto* component : ComponentUpdateCache)
        {
            if (component->Active)
                component->OnUpdate();
        } 
    }

    void DoForEachEntity(size_t compId, std::function<void(Component*)> func)
    {
        auto componentTableItr = ComponentDB.find(compId);
        if (componentTableItr == ComponentDB.end())
            return;

        ComponentTable& componentTable = componentTableItr->second;

        for (auto& entity : componentTable.Entities)
        {
            for(Component* component : entity.second)
                func(component);
        }
    }

    void DoForEachComponentInEntity(uint64_t entityId, std::function<void(Component*)> func)
    {
        for (auto componentTable : ComponentDB)
        {
            auto entityItr = componentTable.second.Entities.find(entityId);
            if (entityItr == componentTable.second.Entities.end())
                continue;

            for (Component* component : entityItr->second)
                func(component);
        }
    }
}
