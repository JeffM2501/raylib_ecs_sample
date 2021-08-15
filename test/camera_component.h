#pragma once

#include "components.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

class CameraComponent : public Component
{
public:
    float FOVY = 45;

public:
    DEFINE_COMPONENT(CameraComponent);
};