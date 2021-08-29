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

#include "lighting_system.h"

#include "light_component.h"
#include "transform_component.h"

#include "raylib.h"
#include <set>

namespace LightingSystem
{
    Shader LightShader;

    std::set<int> UsedLightIds;

#define GLSL_VERSION            330
#define MAX_LIGHTS              4         // Max dynamic lights supported by shader

    Shader& GetShader()
    {
        return LightShader;
    }

    void Setup()
    {
        LightShader = LoadShader(TextFormat("resources/shaders/glsl%i/base_lighting.vs", GLSL_VERSION),
            TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
        
        LightShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(LightShader, "viewPos");

        // Ambient light level (some basic lighting)
        int ambientLoc = GetShaderLocation(LightShader, "ambient");
        
        float color[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

        SetShaderValue(LightShader, ambientLoc, color, SHADER_UNIFORM_VEC4);

        // disable all lights
        char enabledName[32] = "lights[x].enabled\0";
        for (int i = 0; i < MAX_LIGHTS; i++)
        {
            enabledName[7] = '0' + i;

            auto loc = GetShaderLocation(LightShader, enabledName);
            int val = 0;
            SetShaderValue(LightShader, loc, &val, SHADER_UNIFORM_INT);
        }
    }

    void UpdateLights()
    {
        ComponentManager::DoForEachEntity<LightComponent>([](LightComponent* light)
            {
                if (!light->LightEnabled || !light->Active)
                    return;

                if (!light->IsSetup())
                {
                    int id = 0;
                    while (UsedLightIds.find(id) != UsedLightIds.end())
                    {
                        id++;
                        if (id > MAX_LIGHTS)
                            return;
                    }
                    UsedLightIds.insert(id);
                    light->Setup(id, LightShader);
                }
                else
                {
                    light->Update(LightShader);
                }
            });
    }

    void Update(uint64_t cameraEntity)
    {
        auto* transform = ComponentManager::MustGetComponent<TransformComponent>(cameraEntity);
        Vector3 cameraPos = transform->GetWorldPosition();
        float p[3] = { cameraPos.x,cameraPos.y,cameraPos.z };
        SetShaderValue(LightShader, LightShader.locs[SHADER_LOC_VECTOR_VIEW], p, SHADER_UNIFORM_VEC3);
    }

    void SetMaterial(Material& material, Color& albedoColor)
    {
        material.shader = GetShader();
        material.maps[MAP_DIFFUSE].color = albedoColor;
    }
}