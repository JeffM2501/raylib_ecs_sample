#include "components.h"

#include <map>
#include <vector>
#include <algorithm>

using ComponentEntityMap = std::map<uint64_t, Component*>;

namespace ComponentManager
{
    std::map<size_t, ComponentEntityMap> ComponentDB;
    std::vector<Component*> ComponentUpdateCache;

    Component* StoreComponent(size_t compId, Component* component)
    {
        ComponentEntityMap& componentTableItr = ComponentDB[compId];

        auto entityCacheItr = componentTableItr.find(component->EntityId);
        if (entityCacheItr != componentTableItr.end())
        {
            if (entityCacheItr->second != component)
                delete(component);

            return entityCacheItr->second;
        }

        componentTableItr[component->EntityId] = component;
        component->OnCreate();

        if (component->WantUpdate())
            ComponentUpdateCache.push_back(component);

        return component;
    }

    Component* FindComponent(size_t compId, uint64_t entityId)
    {
        auto componentTableItr = ComponentDB.find(compId);
        if (componentTableItr == ComponentDB.end())
            return nullptr;

        ComponentEntityMap& componentTable = componentTableItr->second;

        auto entityCacheItr = componentTable.find(entityId);
        if (entityCacheItr == componentTable.end())
            return nullptr;

        return entityCacheItr->second;
    }

    void EraseComponent(size_t compId, uint64_t entityId)
    {
        auto componentTableItr = ComponentDB.find(compId);
        if (componentTableItr == ComponentDB.end())
            return;

        ComponentEntityMap& componentTable = componentTableItr->second;

        auto entityCacheItr = componentTable.find(entityId);
        if (entityCacheItr != componentTable.end())
        {
            entityCacheItr->second->OnDestory();

            if (entityCacheItr->second->WantUpdate())
                ComponentUpdateCache.erase(std::find(ComponentUpdateCache.begin(), ComponentUpdateCache.end(), entityCacheItr->second));

            delete(entityCacheItr->second);
            componentTable.erase(entityCacheItr);
        }
    }

    void RemoveEntity(uint64_t entityId)
    {
        for (auto componentTable : ComponentDB)
        {
            auto entityCacheItr = componentTable.second.find(entityId);
            if (entityCacheItr != componentTable.second.end())
            {
                delete(entityCacheItr->second);
                componentTable.second.erase(entityCacheItr);
            }
        }
    }

    void Update()
    {
        for (auto* component : ComponentUpdateCache)
            component->OnUpdate();
    }

    void DoForEachEntity(size_t compId, std::function<void(Component*)> func)
    {
        auto componentTableItr = ComponentDB.find(compId);
        if (componentTableItr == ComponentDB.end())
            return;

        ComponentEntityMap& componentTable = componentTableItr->second;

        for (auto& entity : componentTable)
        {
            func(entity.second);
        }
    }
}
