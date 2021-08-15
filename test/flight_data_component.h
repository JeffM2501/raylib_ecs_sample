#pragma once

#include "components.h"

#include "raylib.h"

class FlightDataComponent : public Component
{
public:
    Vector2 LastMousePos;

    float Speed = 10;
    float RotationSpeed = 180;

    bool UseMouseButton = true;
    bool UseHeading = true;

public:
    DEFINE_COMPONENT(FlightDataComponent);

    inline void OnCreate() override
    {
        LastMousePos = GetMousePosition();
    }
};