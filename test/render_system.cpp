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

#include "render_system.h"
#include "lighting_system.h"

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

        Matrix cameraMat = cameraTransform->GetWorldMatrix();
        ViewCam.position = Vector3Transform(Vector3Zero(), cameraMat);
        ViewCam.target = Vector3Transform(Vector3{ 0, 1, 0 }, cameraMat);
        ViewCam.up = Vector3Subtract(Vector3Transform(Vector3{ 0, 0, 1 }, cameraMat), ViewCam.position);

// 
//         // copy the transform vectors to the raylib camera
//         ViewCam.position = cameraTransform->GetPosition();
//         ViewCam.target = Vector3Add(cameraTransform->GetPosition(), cameraTransform->GetForwardVector());
//         ViewCam.up = cameraTransform->GetUpVector();

        BeginMode3D(ViewCam);
    }

    void Draw()
    {
        // TODO, get the visible set
        ComponentManager::DoForEachEntity<Drawable3DComponent>([](Drawable3DComponent* drawable)
            {
                if (drawable->Active)
                    drawable->Draw(ViewCam);
            });
    }

    void End()
    {
        EndMode3D();
    }
}