#pragma once
#include "Polycode.h"
#include "ToolBox.h"

using namespace Polycode;
using namespace ToolBox;

namespace PlayerSpace {
    const float rotationSpeed = 0.3f;
    const Number moveSpeed = 0.05;

    class Player {
    public:
        Player(Camera *defaultCamera);
        ~Player();

        void update();
        void handleInput(CoreInput *input, ScreenLabel *deltaText);
        Vector2 getRotation();
    private:
        void rotateCamera(Vector2 *mouseDifference, float amount);
        void addToCameraPosition(Vector3 *toAdd);

        Vector3 position;
        Vector2 oldDelta;
        float leftRightRot;
        float upDownRot;
        Camera *defaultCamera;
    };
}