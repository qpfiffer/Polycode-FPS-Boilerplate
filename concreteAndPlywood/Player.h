#pragma once
#include "Polycode.h"
#include "ToolBox.h"

using namespace Polycode;
using namespace ToolBox;

namespace PlayerSpace {
    const float rotationSpeed = 0.3f;
    const Number moveSpeed = 0.025;

    class Player {
    public:
        Player(Camera *defaultCamera);
        ~Player();

        void update();
        void pollKeyboardInput(CoreInput *input);
        void handleMouseInput(InputEvent *e);
        Vector2 getRotation();
    private:
        void rotateCamera(Vector2 *mouseDifference, float amount);
        void addToCameraPosition(Vector3 *toAdd);

        bool didRotate;

        Vector3 position;
        Vector2 oldMousePos;
        float leftRightRot;
        float upDownRot;
        Camera *defaultCamera;
    };
}