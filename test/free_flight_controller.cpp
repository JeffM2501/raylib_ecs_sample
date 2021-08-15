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