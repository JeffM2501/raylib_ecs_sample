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
#include "color_component.h"
#include "drawable_component.h"
#include "flight_data_component.h"
#include "look_at_component.h"
#include "transform_component.h"

#include "free_flight_controller.h"
#include "render_system.h"

uint64_t targetEntityId = uint64_t(-1);

void CreateTestEntity()
{
    // add a root component that will create an entity
    TransformComponent* testEntity = ComponentManager::AddComponent<TransformComponent>();
    targetEntityId = testEntity->EntityId;

    testEntity->SetPosition(3, 3, 6);
    testEntity->RotateHeading(45);
    testEntity->RotateRoll(30);
    
    // give it some geometry
    // body
    ShapeComponent* drawable = ComponentManager::AddComponent<ShapeComponent>(testEntity);

    drawable->ObjectSize = Vector3{ 0.25f,0.75f,0.25f };
    drawable->MustGetComponent<ColorComponent>()->SetColor(PURPLE);

    // nose
    drawable = ComponentManager::AddComponent<ShapeComponent>(testEntity);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->MustGetComponent<ColorComponent>()->SetColor(PURPLE);
    drawable->ObjectSize = Vector3{ 0.0625f,0.125f,0.5f };
    drawable->Offset.SetOffset(0, 0.3f, 0 );

    // engine
    drawable = ComponentManager::AddComponent<ShapeComponent>(testEntity);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->MustGetComponent<ColorComponent>()->SetColor(DARKGRAY);
    drawable->ObjectSize = Vector3{ 0.0625f,0.125f,0.125f };
    drawable->Offset.SetOffset( 0, -0.5f, 0 );

    // wings
    drawable = ComponentManager::AddComponent<ShapeComponent>(testEntity);
    drawable->ObjectShape = DrawShape::Plane;
    drawable->MustGetComponent<ColorComponent>()->SetColor(PURPLE);
    drawable->ObjectSize = Vector3{ 1.0f,0.5f,1 };
    drawable->Offset.SetOffset(0, 0, 0.125f );
    drawable->Offset.SetRotation(90, 0, 0 );

    // make it fly around
    AutoMoverComponent* mover = ComponentManager::AddComponent<AutoMoverComponent>(testEntity);
    mover->UseHeading = true;
    mover->LinearSpeed.y = 5;
    mover->AngularSpeed.y = 50;
    mover->AngularSpeed.z = 20;

    // add an animated component
    TransformComponent* radarDish = ComponentManager::AddComponent<TransformComponent>();
    testEntity->AddChild(radarDish);
    radarDish->SetPosition(0, 0, 0.35f);

    // radar dish
    drawable = ComponentManager::AddComponent<ShapeComponent>(radarDish);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->MustGetComponent<ColorComponent>()->SetColor(SKYBLUE);
    drawable->Offset.SetOffset(0, 0, 0.125f);
    drawable->ObjectSize = Vector3{ 0.25f,0.125f,0.125f };

    // put the dish on a pole
    drawable = ComponentManager::AddComponent<ShapeComponent>(radarDish);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->MustGetComponent<ColorComponent>()->SetColor(GRAY);
    drawable->Offset.SetOffset(0, -0.25f, 0);
    drawable->Offset.SetRotation(90, 0, 0);
    drawable->ObjectSize = Vector3{ 0.025f,0.025f,0.35f };

    // make it spin
    mover = ComponentManager::AddComponent<AutoMoverComponent>(radarDish);
    mover->AngularSpeed.y = 180;
}

std::vector<TransformComponent*> Cameras;
size_t cameraIndex = 0;

void CreateCameras()
{
    // the free flight camera
    TransformComponent* camera = ComponentManager::AddComponent<TransformComponent>();
    ComponentManager::AddComponent<CameraComponent>(camera);
    ComponentManager::AddComponent<FlightDataComponent>(camera);

    camera->SetPosition(0, -1, 1);

    // cone for visualization
    auto drawable = ComponentManager::AddComponent<ShapeComponent>(camera);
    drawable->MustGetComponent<ColorComponent>()->SetColor(DARKGREEN);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->ObjectSize = Vector3{ 0.5f, 0.125f, 0.5f };

    Cameras.push_back(camera);

    // the look at camera
    camera = ComponentManager::AddComponent<TransformComponent>();
    camera->SetPosition(0, 1, 2);
    ComponentManager::AddComponent<CameraComponent>(camera);
    ComponentManager::AddComponent<LookAtComponent>(camera)->TargetEntityId = targetEntityId;

    // body
    drawable = ComponentManager::AddComponent<ShapeComponent>(camera);
    drawable->MustGetComponent<ColorComponent>()->SetColor(DARKGRAY);
    drawable->ObjectShape = DrawShape::Box;
    drawable->ObjectSize = Vector3{ 0.125f,0.5f,0.25f };

    // lens
    drawable = ComponentManager::AddComponent<ShapeComponent>(camera);
    drawable->MustGetComponent<ColorComponent>()->SetColor(DARKGRAY);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->ObjectSize = Vector3{ 0.125f, 0, 0.5f };
    drawable->Offset.SetOffset(0, -0.001f, 0);

    // reels
    drawable = ComponentManager::AddComponent<ShapeComponent>(camera);
    drawable->MustGetComponent<ColorComponent>()->SetColor(DARKGRAY);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->ObjectSize = Vector3{ 0.25f, 0.25f, 0.0625f };
    drawable->Offset.SetRotation(0,0,90);
    drawable->Offset.SetOffset(0.25f, 0, 0.4f);

    drawable = ComponentManager::AddComponent<ShapeComponent>(camera);
    drawable->MustGetComponent<ColorComponent>()->SetColor(DARKGRAY);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->ObjectSize = Vector3{ 0.25f, 0.25f, 0.0625f };
    drawable->Offset.SetRotation(0,0,90);
    drawable->Offset.SetOffset(-0.25f, 0, 0.4f );

    Cameras.push_back(camera);

    // static camera
    camera = ComponentManager::AddComponent<TransformComponent>();
    ComponentManager::AddComponent<CameraComponent>(camera);

    camera->SetPosition(-10, -10, 5);
    camera->LookAt(Vector3{ 6, 1, 3 }, Vector3{ 0,0,1 });

    // cone for visualization
    drawable = ComponentManager::AddComponent<ShapeComponent>(camera);
    drawable->MustGetComponent<ColorComponent>()->SetColor(DARKBLUE);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->ObjectSize = Vector3{ 0.5f, 0.125f, 0.5f };

    Cameras.push_back(camera);
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
    CreateCameras();

    while (!WindowShouldClose())
    {
        ComponentManager::Update();

        FreeFlightController::Update(Cameras[0]);

        if (IsKeyPressed(KEY_SPACE))
            cameraIndex += 1;

        if (cameraIndex >= Cameras.size())
            cameraIndex = 0;

        BeginDrawing();
        ClearBackground(BLACK);

        RenderSystem::Begin(Cameras[cameraIndex]->EntityId);
        DrawGrid();
        
        RenderSystem::Draw();

        RenderSystem::End();

        DrawText(TextFormat("X%.2f Y%.2f Z%.2f", cameraIndex, Cameras[cameraIndex]->GetPosition().x, Cameras[cameraIndex]->GetPosition().y, Cameras[cameraIndex]->GetPosition().z),0,20,20,RED);
        DrawText("Space to change cameras", 0, 40, 20, RED);
        switch (cameraIndex)
        {
        case 0:
            DrawText("Free Flight Camera", 0, 60, 20, RED);
            DrawText("RIGHT DRAG to rotate view, WASD to move", 0, 80, 20, RED);
            break;
        case 1:
            DrawText("Tracking Camera", 0, 60, 20, RED);
            break;
        case 2:
            DrawText("Static Camera", 0, 60, 20, RED);
            break;
        }

        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
}