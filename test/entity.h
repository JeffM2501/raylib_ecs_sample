#pragma once

#include <stdint.h>

namespace EntityManger
{
    uint64_t CreateEntity();
    void ReleaseEntity(uint64_t id);
}