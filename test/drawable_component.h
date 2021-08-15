#pragma once

#include "components.h"
#include "transform_component.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

enum class DrawShape
{
    Box,
    Sphere,
    Cylinder,
    Plane
};

class DrawableComponent : public Component
{
public:
    Vector3 ObjectSize = { 1, 1, 1 };
    Color ObjectColor = WHITE;
    DrawShape ObjectShape = DrawShape::Box;

    Vector3 ObjectOrigin = { 0, 0,0 };

public:
    DEFINE_COMPONENT(DrawableComponent);

    inline virtual void Draw()
    {
        TransformComponent* transform = ComponentManager::GetComponent<TransformComponent>(this);
        if (transform == nullptr)
            return;

        transform->PushMatrix();

        switch (ObjectShape)
        {
        case DrawShape::Box:
            DrawCube(ObjectOrigin, ObjectSize.x, ObjectSize.y, ObjectSize.z, ObjectColor);
            break;
        case DrawShape::Sphere:
            DrawSphere(ObjectOrigin, std::max(std::max(ObjectSize.x, ObjectSize.y), ObjectSize.z), ObjectColor);
;           break;
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