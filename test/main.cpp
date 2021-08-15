#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <vector>
#include <functional>

#include "automover_component.h"
#include "camera_component.h"
#include "drawable_component.h"
#include "flight_data_component.h"
#include "transform_component.h"

#include "free_flight_controller.h"
#include "render_system.h"


void CreateTestEntity()
{
    TransformComponent* testEntity = ComponentManager::AddComponent<TransformComponent>();
    testEntity->SetPosition(3, 3, 3);
    testEntity->RotateHeading(45);
    testEntity->RotatePitch(30);

    DrawableComponent* drawable = ComponentManager::AddComponent<DrawableComponent>(testEntity);
    drawable->ObjectColor = PURPLE;
    drawable->ObjectSize = Vector3{ 0.25f,0.75f,0.25f };

    AutoMoverComponent* mover = ComponentManager::AddComponent<AutoMoverComponent>(testEntity);
    mover->LinearSpeed.y = 5;
    mover->AngularSpeed.y = 90;

    TransformComponent* child = ComponentManager::AddComponent<TransformComponent>();
    testEntity->AddChild(child);
    child->SetPosition(0, 0, 0.35f);

    mover = ComponentManager::AddComponent<AutoMoverComponent>(child);
    mover->AngularSpeed.y = 180;

    drawable = ComponentManager::AddComponent<DrawableComponent>(child);
    drawable->ObjectShape = DrawShape::Cylinder;
    drawable->ObjectColor = SKYBLUE;
    drawable->ObjectSize = Vector3{ 0.25f,0.125f,0.125f };
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
    InitWindow(1280, 900, "Test");

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