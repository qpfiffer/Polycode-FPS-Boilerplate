#pragma once
#include "Polycode.h"

using namespace Polycode;

namespace ToolBox {

    const double M_PI = 3.141592;

    double ToRadians(double d);
    double ToDegrees(double r);
    Vector2 subtract(Vector2 one, Vector2 two);
    float abs(float input);

    Matrix4 CreateRotationX(float degrees);
    Matrix4 CreateRotationY(float degrees);
    Matrix4 CreateRotationZ(float degrees);
}