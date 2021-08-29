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

#pragma once

#include "components.h"

#include "raylib.h"

constexpr float ByteToFloat = 1.0f / 255.0f;

class ColorComponent : public Component
{
private:
    Color ColorValue = WHITE;
    float GLColor[4] = { 1,1,1,1 };

    inline void UpdateGLColor()
    {
        GLColor[0] = ColorValue.r * ByteToFloat;
        GLColor[1] = ColorValue.g * ByteToFloat;
        GLColor[2] = ColorValue.b * ByteToFloat;
        GLColor[3] = ColorValue.a * ByteToFloat;
    }

public:
    DEFINE_COMPONENT(ColorComponent);

    void SetColor(Color c) { ColorValue = c; UpdateGLColor(); }

    Color GetColor() const { return ColorValue; }
    const float* GetGLColor() const { return GLColor; }
};