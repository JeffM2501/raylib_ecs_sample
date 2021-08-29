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

#include "light_component.h"
#include "transform_component.h"
#include "color_component.h"

void LightComponent::Setup(int index, Shader& shader)
{
    LightIndex = index;
    LightEnabled = 1;

    // TODO: Below code doesn't look good to me, 
    // it assumes a specific shader naming and structure
    // Probably this implementation could be improved
    char enabledName[32] = "lights[x].enabled\0";
    char typeName[32] = "lights[x].type\0";
    char posName[32] = "lights[x].position\0";
    char targetName[32] = "lights[x].target\0";
    char colorName[32] = "lights[x].color\0";

    // Set location name [x] depending on lights count
    enabledName[7] = '0' + LightIndex;
    typeName[7] = '0' + LightIndex;
    posName[7] = '0' + LightIndex;
    targetName[7] = '0' + LightIndex;
    colorName[7] = '0' + LightIndex;

    EnabledLoc = GetShaderLocation(shader, enabledName);
    TypeLoc = GetShaderLocation(shader, typeName);
    PosLoc = GetShaderLocation(shader, posName);
    TargetLoc = GetShaderLocation(shader, targetName);
    ColorLoc = GetShaderLocation(shader, colorName);

    Update(shader);
}

void LightComponent::Update(Shader& shader)
{
    // Send to shader light enabled state and type
    SetShaderValue(shader, EnabledLoc, &LightEnabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, TypeLoc, &LightType, SHADER_UNIFORM_INT);

    auto* transform = MustGetComponent<TransformComponent>();

    Vector3 pos = transform->GetWorldPosition();
    float p[3] = { pos.x,pos.y,pos.z };
   
    // Send to shader light position values
    SetShaderValue(shader, PosLoc, p, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    Vector3 target = { 0 };
    if (LightType == LightTypes::DIRECTIONAL)
        target = transform->GetWorldTarget();

    float t[3] = { target.x,target.y,target.z };

    SetShaderValue(shader, TargetLoc, &target.x, SHADER_UNIFORM_VEC3);

    SetShaderValue(shader, ColorLoc, MustGetComponent<ColorComponent>()->GetGLColor(), SHADER_UNIFORM_VEC4);
}
