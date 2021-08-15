#include "entity.h"

#include <set>



namespace EntityManger
{
    std::set<uint64_t> EntityMap;
    uint64_t NextEntity = 0;

    uint64_t CreateEntity()
    {
        while (EntityMap.find(NextEntity) != EntityMap.end())
            NextEntity++;

        EntityMap.emplace(NextEntity);
        NextEntity++;

        return NextEntity - 1;
    }

    void ReleaseEntity(uint64_t id)
    {
        auto itr = EntityMap.find(id);
        if (itr == EntityMap.end())
            return;

        EntityMap.erase(itr);
    }
}