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

// a component that uses the update event

class AutoMoverComponent : public Component
{
public:
    Vector3 LinearSpeed = { 0 };
    Vector3 AngularSpeed = { 0 };

    bool UseHeading = false;

public:
    DEFINE_COMPONENT(AutoMoverComponent);

    inline void OnCreate() override { NeedUpdate = true; }

    inline void OnUpdate()
    {
        TransformComponent* transform = ComponentManager::MustGetComponent<TransformComponent>(this);

        float delta = GetFrameTime();

        transform->RotatePitch(AngularSpeed.x * delta);
        if (UseHeading)
            transform->RotateHeading(AngularSpeed.y * delta);
        else
            transform->RotateYaw(AngularSpeed.y * delta);
        transform->RotateRoll(AngularSpeed.z * delta);

        transform->MoveRight(LinearSpeed.x * delta);
        transform->MoveForward(LinearSpeed.y * delta);
        transform->MoveUp(LinearSpeed.z * delta);
    }
};