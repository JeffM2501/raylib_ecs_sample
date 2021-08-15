#pragma once

#include "components.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <vector>

class TransformComponent : public Component
{
private:
    Vector3 Position = { 0 };
    Vector3 Forward = { 0 , 1 , 0 };
    Vector3 Up = { 0, 0 , 1 };

    bool Dirty = true;

    Matrix WorldMatrix = { 0 };
    Matrix GlWorldMatrix = { 0 };

public:
    DEFINE_COMPONENT(TransformComponent);

    TransformComponent* Parent = nullptr;

    std::vector<TransformComponent*> Children;

    void AddChild(TransformComponent* child)
    {
        Children.push_back(child);
        child->Parent = this;
    }

    void RemoveChild(TransformComponent* child)
    {
        auto itr = std::find(Children.begin(), Children.end(), child);
        if (itr != Children.end())
            Children.erase(itr);

        if (child->Parent == this)
            child->Parent = nullptr;
    }

    void Detach()
    {
        if (Parent == nullptr)
            return;

        Matrix worldTransform = GetWorldMatrix();
        Position = Vector3Transform(Vector3Zero(), WorldMatrix);

        Matrix translateMatrix = MatrixTranslate(Position.x, Position.y, Position.z);
        Matrix orientationMatrix = MatrixMultiply(worldTransform, translateMatrix);

        Forward = Vector3Transform(Vector3{ 0 , 1 , 0 }, WorldMatrix);
        Up = Vector3Transform(Vector3{ 0, 0 , 1 }, WorldMatrix);

        Parent->RemoveChild(this);
    }

    const Vector3& GetPosition() const { return Position; }
    const Vector3& GetForwardVector() const { return Forward; }
    const Vector3& GetUpVector() const { return Up; }

    // destroy this entity (and all other components) and all children
    void DestoryWithChilren()
    {
        for (TransformComponent* child : Children)
        {
            child->DestoryWithChilren();
        }

        if (Parent != nullptr)
            Parent->RemoveChild(this);

        EntityManger::ReleaseEntity(EntityId);
    }

    void SetPosition(float x, float y, float z)
    {
        Position.x = x;
        Position.y = y;
        Position.z = z;
        Dirty = true;
    }

    void LookAt(const Vector3& target, const Vector3& up)
    {
        Forward = Vector3Normalize(Vector3Subtract(target, Position));
        Up = Vector3Normalize(up);
    }

    bool IsDirty()
    {
        if (Parent != nullptr)
            return Parent->IsDirty() || Dirty;

        return Dirty;
    }

    Matrix GetLocalMatrix()
    {
        Matrix orient = MatrixLookAt(Vector3Zero(), Forward, Up);
        // keep local Z up
        orient = MatrixMultiply(orient, MatrixRotateX(-90 * DEG2RAD));
        Matrix translation = MatrixTranslate(Position.x, Position.y, Position.z);

        return MatrixMultiply(MatrixInvert(orient), translation);
    }

    void UpdateWorldMatrix()
    {
        Matrix parentMatrix = MatrixIdentity();
        if (Parent != nullptr)
            parentMatrix = Parent->GetWorldMatrix();

        WorldMatrix = MatrixMultiply(GetLocalMatrix(), parentMatrix);
        GlWorldMatrix = MatrixTranspose(WorldMatrix);

        Dirty = false;
    }

    const Matrix& GetWorldMatrix()
    {
        if (!IsDirty())
            return WorldMatrix;

        UpdateWorldMatrix();
        return WorldMatrix;
    }

    const Matrix& GetGLWorldMatrix()
    {
        if (!IsDirty())
            return GlWorldMatrix;

        UpdateWorldMatrix();
        return GlWorldMatrix;
    }

    Vector3 ToLocalPos(const Vector3& inPos)
    {
        return Vector3Transform(inPos, MatrixInvert(GetWorldMatrix()));
    }

    Vector3 GetLeftVector()
    {
        return Vector3CrossProduct(Up, Forward);
    }

    Vector3 GetRightVector()
    {
        return Vector3CrossProduct(Forward, Up);
    }

    void MoveUp(float distance)
    {
        Dirty = true;
        Position = Vector3Add(Position, Vector3Scale(Up, distance));
    }

    void MoveDown(float distance)
    {
        Dirty = true;
        Position = Vector3Add(Position, Vector3Scale(Up, -distance));
    }

    void MoveForward(float distance)
    {
        Dirty = true;
        Position = Vector3Add(Position, Vector3Scale(Forward, distance));
    }

    void MoveBackwards(float distance)
    {
        Dirty = true;
        Position = Vector3Add(Position, Vector3Scale(Forward, -distance));
    }

    void MoveLeft(float distance)
    {
        Dirty = true;
        Position = Vector3Add(Position, Vector3Scale(GetLeftVector(), distance));
    }

    void MoveRight(float distance)
    {
        MoveLeft(-distance);
    }

    void RotateYaw(float angle)
    {
        Dirty = true;
        auto matrix = MatrixRotate(Up, DEG2RAD * angle);
        Forward = Vector3Normalize(Vector3Transform(Forward, matrix));
    }

    void RotatePitch(float angle)
    {
        Dirty = true;

        angle = fmodf(angle, 360);
        if (angle < 0)
            angle += 360;
        else if (angle > 360)
            angle -= 360;

        auto matrix = MatrixRotate(GetLeftVector(), DEG2RAD * angle);

        Up = Vector3Normalize(Vector3Transform(Up, matrix));
        Forward = Vector3Normalize(Vector3Transform(Forward, matrix));
    }

    void RotateRoll(float angle)
    {
        Dirty = true;
        auto matrix = MatrixRotate(Forward, DEG2RAD * angle);
        Up = Vector3Normalize(Vector3Transform(Up, matrix));
    }

    void RotateHeading(float angle)
    {
        Dirty = true;
        Matrix matrix = MatrixRotateZ(DEG2RAD * angle);
        Up = Vector3Normalize(Vector3Transform(Up, matrix));
        Forward = Vector3Normalize(Vector3Transform(Forward, matrix));
    }

    void PushMatrix()
    {
        const Matrix& glMatrix = GetGLWorldMatrix();
        rlPushMatrix();
        rlMultMatrixf((float*)(&glMatrix.m0));
    }

    void PopMatrix()
    {
        rlPopMatrix();
    }
};