#include "render_system.h"

#include "drawable_component.h"
#include "transform_component.h"
#include "camera_component.h"

#include "raylib.h"

namespace RenderSystem
{
    Camera3D ViewCam = { 0 };

    void Begin(uint64_t cameraEntityId)
    {
        CameraComponent* camera = ComponentManager::MustGetComponent<CameraComponent>(cameraEntityId);
        ViewCam.fovy = 45;

        // a camera entity must have a the transform component, if it doesn't we add one and get the default
        TransformComponent* cameraTransform = ComponentManager::MustGetComponent<TransformComponent>(camera);

        // copy the transform vectors to the raylib camera
        ViewCam.position = cameraTransform->GetPosition();
        ViewCam.target = Vector3Add(cameraTransform->GetPosition(), cameraTransform->GetForwardVector());
        ViewCam.up = cameraTransform->GetUpVector();

        BeginMode3D(ViewCam);
    }

    void Draw()
    {
        // TODO, get the visible set

        ComponentManager::DoForEachEntity<DrawableComponent>([](DrawableComponent* drawable)
            {
                drawable->Draw();
            });
    }

    void End()
    {
        EndMode3D();
    }
}