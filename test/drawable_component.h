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

#include "color_component.h"

class Drawable3DComponent : public Component
{
public:
    DEFINE_COMPONENT(Drawable3DComponent);

    inline virtual void Draw(const Camera3D& camera) {}
};

enum class DrawShape
{
    Box,
    Sphere,
    Cylinder,
    Plane
};

class OffsetTransform
{
public:
    inline void Apply()
    {
        if (IsZero)
            return;

        rlRotatef(Rotation.x, 1, 0, 0);
        rlRotatef(Rotation.y, 0, 1, 0);
        rlRotatef(Rotation.z, 0, 0, 1);

        rlTranslatef(Offset.x, Offset.y, Offset.z);
    }

    inline void SetOffset(float x, float y, float z)
    {
        Offset = { x,y,z };
        CheckZero();
    }

    inline void SetRotation(float x, float y, float z)
    {
        Rotation = { x, y ,z };
        CheckZero();
    }

private:
    Vector3 Offset = { 0,0,0 };
    Vector3 Rotation{ 0,0,0 };

    void CheckZero()
    {
        IsZero = (Offset.x == 0 && Offset.y == 0 && Offset.z == 0 && Rotation.x == 0 && Rotation.y == 0 && Rotation.z == 0);
    }

    bool IsZero = true;
};

class ShapeComponent : public Drawable3DComponent
{
public:
    Vector3 ObjectSize = { 1, 1, 1 };
    DrawShape ObjectShape = DrawShape::Box;

    OffsetTransform Offset;

public:
    DEFINE_DERIVED_COMPONENT(ShapeComponent, Drawable3DComponent);

    inline void Draw(const Camera3D&) override
    {
        TransformComponent* transform = GetComponent<TransformComponent>();
        if (transform == nullptr)
            return;

        transform->PushMatrix();

        Offset.Apply();

        Color objectColor = MustGetComponent<ColorComponent>()->GetColor();

        switch (ObjectShape)
        {
        case DrawShape::Box:
            DrawCube(Vector3Zero(), ObjectSize.x, ObjectSize.y, ObjectSize.z, objectColor);
            break;
        case DrawShape::Sphere:
            DrawSphere(Vector3Zero(), std::max(std::max(ObjectSize.x, ObjectSize.y), ObjectSize.z), objectColor);
            ;           break;
        case DrawShape::Cylinder:
            DrawCylinder(Vector3Zero(), ObjectSize.x, ObjectSize.y, ObjectSize.z, 32, objectColor);
            break;
        case DrawShape::Plane:
            rlDisableBackfaceCulling();
            DrawPlane(Vector3Zero(), Vector2{ ObjectSize.x,ObjectSize.y }, objectColor);
            rlDrawRenderBatchActive();
            rlEnableBackfaceCulling();
            break;
        default:
            break;
        }

        transform->PopMatrix();
    }
};

class MeshComponent : public Drawable3DComponent
{
public:
    Mesh ObjetMesh;
    Material ObjectMaterial;
    OffsetTransform Offset;

    bool UseColor = false;
 
public:

    DEFINE_DERIVED_COMPONENT(MeshComponent, Drawable3DComponent);

    inline void OnCreate() override
    {
        ObjectMaterial = LoadMaterialDefault();
    }

    inline void Draw(const Camera3D&) override
    {
        TransformComponent* transform = GetComponent<TransformComponent>();
        if (transform == nullptr)
            return;

        transform->PushMatrix();

        Offset.Apply();

        if (UseColor)
        {
            ColorComponent* color = GetComponent<ColorComponent>();
            if (color != nullptr)
                ObjectMaterial.maps[0].color = color->GetColor();
        }

        DrawMesh(ObjetMesh, ObjectMaterial, MatrixIdentity());

        transform->PopMatrix();
    }
};