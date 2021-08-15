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

#include "raylib.h"

#include "automover_component.h"
#include "camera_component.h"
#include "drawable_component.h"
#include "flight_data_component.h"
#include "transform_component.h"

#include "free_flight_controller.h"
#include "render_system.h"

void CreateTestEntity()
{
    // add a root component that will create an entity
    TransformComponent* testEntity = ComponentManager::AddComponent<TransformComponent>();
    testEntity->SetPosition(3, 3, 3);
    testEntity->RotateHeading(45);
    testEntity->RotatePitch(30);
    
    // give it some geometry
    DrawableComponent* drawable = ComponentManager::AddComponent<DrawableComponent>(testEntity);
    drawable->ObjectColor = PURPLE;
    drawable->ObjectSize = Vector3{ 0.25f,0.75f,0.25f };

    drawable = ComponentManager::AddComponent<DrawableComponent>(testEntity);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->ObjectColor = PURPLE;
    drawable->ObjectSize = Vector3{ 0.0625f,0.125f,0.5f };
    drawable->ObjectOrigin = Vector3{ 0, 0.3f, 0 };

    drawable = ComponentManager::AddComponent<DrawableComponent>(testEntity);
    drawable->ObjectShape = DrawShape::Plane;
    drawable->ObjectColor = PURPLE;
    drawable->ObjectSize = Vector3{ 1.0f,0.5f,1 };
    drawable->ObjectOrigin = Vector3{ 0, 0, 0.125f };
    drawable->ObjectOrientationShift = Vector3{ 90, 0, 0 };

    // make it fly around
    AutoMoverComponent* mover = ComponentManager::AddComponent<AutoMoverComponent>(testEntity);
    mover->LinearSpeed.y = 5;
    mover->AngularSpeed.y = 90;

    // add a radar dish
    TransformComponent* radarDish = ComponentManager::AddComponent<TransformComponent>();
    testEntity->AddChild(radarDish);
    radarDish->SetPosition(0, 0, 0.35f);

    // give it some geometry
    drawable = ComponentManager::AddComponent<DrawableComponent>(radarDish);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->ObjectColor = SKYBLUE;
    drawable->ObjectSize = Vector3{ 0.25f,0.125f,0.125f };

    // make it spin
    mover = ComponentManager::AddComponent<AutoMoverComponent>(radarDish);
    mover->AngularSpeed.y = 180;
}

TransformComponent* CreateCamera()
{
    TransformComponent* camera = ComponentManager::AddComponent<TransformComponent>();
    ComponentManager::AddComponent<CameraComponent>(camera);
    ComponentManager::AddComponent<FlightDataComponent>(camera);

    return camera;
}

void DrawGrid()
{
    // world grid
    rlPushMatrix();
    rlRotatef(90, 1, 0, 0);
    DrawGrid(50, 2.5f);
    rlPopMatrix();

    for (float y = -20; y <= 20; y += 5)
    {
        for (float x = -20; x <= 20; x += 5)
        {
            Color c = DARKGRAY;
            if (x > 0)
            {
                if (y > 0)
                    c = BLUE;
                else
                    c = PURPLE;
            }
            else
            {
                if (y > 0)
                    c = RED;
                else
                    c = ORANGE;
            }

            DrawCube(Vector3{ x,y, 0 }, 0.5f, 0.5f, 0, c);
        }
    }

    for (float z = -30; z <= 30; z += 2)
    {
        if (z == 0)
            continue;

        Color c = BROWN;
        if (z < 0)
            c = GRAY;

        DrawCube(Vector3{ 0,0,z }, 0.125f, 0.125f, 0.125f, c);
    }

}

void main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1280, 900, "ECS Test");

    SetTargetFPS(144);

    CreateTestEntity();
    TransformComponent* camera = CreateCamera();

    while (!WindowShouldClose())
    {
        ComponentManager::Update();
        FreeFlightController::Update(camera);

        BeginDrawing();
        ClearBackground(BLACK);

        RenderSystem::Begin(camera->EntityId);
        DrawGrid();
        
        RenderSystem::Draw();

        RenderSystem::End();

        DrawText(TextFormat("X%.2f Y%.2f Z%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z),0,20,20,RED);

        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
}