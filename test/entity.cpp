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

    void DoForEach(std::function<void(uint64_t)> func)
    {
        for (uint64_t entity : EntityMap)
            func(entity);
    }
}