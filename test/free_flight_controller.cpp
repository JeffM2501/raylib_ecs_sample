#include "free_flight_controller.h"

#include "flight_data_component.h"

#include "raylib.h"

namespace FreeFlightController
{
    void Update(TransformComponent* toMove)
    {
        if (toMove == nullptr)
            return;

        FlightDataComponent* flightData = ComponentManager::MustGetComponent<FlightDataComponent>(toMove);

        float speed = flightData->Speed * GetFrameTime();

        if (IsKeyDown(KEY_LEFT_SHIFT))
            speed *= 5.0f;

        if (IsKeyDown(KEY_Q))
            toMove->MoveUp(speed);
        else if (IsKeyDown(KEY_E))
            toMove->MoveUp(-speed);

        if (IsKeyDown(KEY_D))
            toMove->MoveRight(speed);
        else if (IsKeyDown(KEY_A))
            toMove->MoveLeft(speed);

        if (IsKeyDown(KEY_W))
            toMove->MoveForward(speed);
        else if (IsKeyDown(KEY_S))
            toMove->MoveForward(-speed);

        float rotSpeed = flightData->RotationSpeed * GetFrameTime();

        if (flightData->UseHeading)
        {
            if (IsKeyDown(KEY_RIGHT))
                toMove->RotateHeading(rotSpeed);
            else if (IsKeyDown(KEY_LEFT))
                toMove->RotateHeading(-rotSpeed);
        }
        else
        {
            if (IsKeyDown(KEY_RIGHT))
                toMove->RotateYaw(-rotSpeed);
            else if (IsKeyDown(KEY_LEFT))
                toMove->RotateYaw(rotSpeed);
        }

        if (IsKeyDown(KEY_UP))
            toMove->RotatePitch(-rotSpeed);
        else if (IsKeyDown(KEY_DOWN))
            toMove->RotatePitch(rotSpeed);

        if (!flightData->UseMouseButton || IsMouseButtonDown(1))
        {
            Vector2 delta = Vector2Subtract(GetMousePosition(), flightData->LastMousePos);

            toMove->RotateHeading(delta.x * rotSpeed * 0.2f);
            toMove->RotatePitch(delta.y * rotSpeed * 0.2f);
        }

        if (IsKeyDown(KEY_DELETE))
            toMove->RotateRoll(-rotSpeed);
        else if (IsKeyDown(KEY_PAGE_DOWN))
            toMove->RotateRoll(rotSpeed);

        flightData->LastMousePos = GetMousePosition();
    }
}