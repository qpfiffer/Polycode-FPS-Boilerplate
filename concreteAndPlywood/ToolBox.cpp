#include "ToolBox.h"
#include <cmath>

using namespace ToolBox;

double ToolBox::ToRadians(double d) {
    return d * M_PI / 180;
}

double ToolBox::ToDegrees(double r) {
    return r * 180/ M_PI;
}

// Subtracts two from one. (For some reason using the - operator on two
// Vector2s gives you a Vector3. Lol.)
Vector2 ToolBox::subtract(Vector2 one, Vector2 two) {
    Vector2 toReturn;
    toReturn.x = two.x - one.x;
    toReturn.y = two.y - one.y;

    return toReturn;
}

// Absolute value.
float ToolBox::abs(float input) {
    if (input < 0.0f)
        return (-1.0f * input);

    return input;
}

Matrix4 ToolBox::CreateRotationY(float degrees) {
    float c = cos(degrees);
    float s = sin(degrees);
    Matrix4 *toReturn = new Matrix4(
        c, 0, s, 0,
        0, 1, 0, 0,
        -s, 0, c, 0,
        0, 0, 0, 1);

    return *toReturn;
}

Matrix4 ToolBox::CreateRotationX(float degrees) {
    float c = cos(degrees);
    float s = sin(degrees);
    Matrix4 *toReturn = new Matrix4(
        1, 0, 0, 0,
        0, c, -s, 0,
        0, s, c, 0,
        0, 0, 0, 1);

    return *toReturn;
}

Matrix4 ToolBox::CreateRotationZ(float degrees) {
    float c = cos(degrees);
    float s = sin(degrees);
    Matrix4 *toReturn = new Matrix4(
        c, -s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);

    return *toReturn;
}