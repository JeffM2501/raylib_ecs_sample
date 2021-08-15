#pragma once

#include "entity.h"
#include "components.h"

namespace RenderSystem
{
    void Begin(uint64_t cameraEntityId);
    void Draw();
    void End();
}