#pragma once

#include "components.h"
#include "transform_component.h"

#include "raylib.h"

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