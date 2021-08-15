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

#include "components.h"
#include "transform_component.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

class LookAtComponent : public Component
{
public:
    uint64_t TargetEntityId = 0;

public:
    DEFINE_COMPONENT(LookAtComponent);

    inline void OnCreate() override { NeedUpdate = true; }

    inline void SetTarget(Component* component)
    {
        if (component == nullptr)
            TargetEntityId = 0;
        else
            TargetEntityId = component->EntityId;
    }

    inline void OnUpdate()
    {
        if (TargetEntityId == uint64_t(-1))
            return;

        TransformComponent* selfTransform = ComponentManager::MustGetComponent<TransformComponent>(this);

        TransformComponent* taretTransform = ComponentManager::MustGetComponent<TransformComponent>(TargetEntityId);

        Vector3 targetPos = Vector3Transform(Vector3Zero(), taretTransform->GetWorldMatrix());

        selfTransform->LookAt(targetPos, Vector3{ 0,0,1 });
    }
};