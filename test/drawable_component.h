#pragma once

#include "components.h"
#include "transform_component.h"

#include "raylib.h"
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

#include "raymath.h"
#include "rlgl.h"

class DrawableComponent : public Component
{
public:
    DEFINE_COMPONENT(DrawableComponent);

    inline virtual void Draw() {}
};

enum class DrawShape
{
    Box,
    Sphere,
    Cylinder,
    Plane
};

class ShapeComponent : public DrawableComponent
{
public:
    Vector3 ObjectSize = { 1, 1, 1 };
    Color ObjectColor = WHITE;
    DrawShape ObjectShape = DrawShape::Box;

    Vector3 ObjectOrigin = { 0, 0,0 };
    Vector3 ObjectOrientationShift = { 0, 0 ,0 };

public:

    DEFINE_DERIVED_COMPONENT(ShapeComponent, DrawableComponent);

    inline void Draw() override
    {
        TransformComponent* transform = ComponentManager::GetComponent<TransformComponent>(this);
        if (transform == nullptr)
            return;

        transform->PushMatrix();

        rlRotatef(ObjectOrientationShift.x, 1, 0, 0);
        rlRotatef(ObjectOrientationShift.y, 0, 1, 0);
        rlRotatef(ObjectOrientationShift.z, 0, 0, 1);

        switch (ObjectShape)
        {
        case DrawShape::Box:
            DrawCube(ObjectOrigin, ObjectSize.x, ObjectSize.y, ObjectSize.z, ObjectColor);
            break;
        case DrawShape::Sphere:
            DrawSphere(ObjectOrigin, std::max(std::max(ObjectSize.x, ObjectSize.y), ObjectSize.z), ObjectColor);
            break;
        case DrawShape::Cylinder:
            DrawCylinder(ObjectOrigin, ObjectSize.x, ObjectSize.y, ObjectSize.z, 32, ObjectColor);
            break;
        case DrawShape::Plane:
            rlDisableBackfaceCulling();
            DrawPlane(ObjectOrigin, Vector2{ ObjectSize.x,ObjectSize.y }, ObjectColor);
            rlDrawRenderBatchActive();
            rlEnableBackfaceCulling();
            break;
        default:
            break;
        }

        transform->PopMatrix();
    }
};
