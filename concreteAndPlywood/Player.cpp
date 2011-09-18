#include "Player.h"

using namespace PlayerSpace;
using namespace ToolBox;

Player::Player(Camera *defaultCamera): 
    position(Vector3(0,0.5,0)), oldMousePos(Vector2(0,0)), 
    leftRightRot(0.0f), upDownRot(0.0f), defaultCamera(defaultCamera)
{
}

Player::~Player() {
    // Pretty sure this is deleted somewhere else.
    defaultCamera = nullptr;
}

void Player::update() {
    
}

void Player::rotateCamera(Vector2 *mouseDifference, float amount)  {
    this->leftRightRot -= rotationSpeed * mouseDifference->x * amount;
    if (leftRightRot > 360.0f)
        leftRightRot -= (float)(360.0f);
    else if (leftRightRot < -(360.0f))
        leftRightRot += (float)(360.0f);

    if (upDownRot - (rotationSpeed * mouseDifference->y * amount) < 90.0f &&
        upDownRot - (rotationSpeed * mouseDifference->y * amount) > -(90.0f))
    {
        upDownRot -= rotationSpeed * mouseDifference->y * amount;
    }

    this->defaultCamera->setYaw(-this->leftRightRot);
    this->defaultCamera->setPitch(-this->upDownRot);
}

void Player::addToCameraPosition(Vector3 *toAdd) {
    // Set the temporary camera position variable:
    Vector3 oldCamera = position;

    Matrix4 cameraRotation;

    cameraRotation = ToolBox::CreateRotationX(ToolBox::ToRadians(leftRightRot)) *
        ToolBox::CreateRotationY(0.0f);

    //Vector3 rotatedVector = Vector3.Transform(vectorToAdd, cameraRotation);
    Vector3 rotatedVector = cameraRotation.rotateVector(*toAdd);
    oldCamera += (rotatedVector * moveSpeed);

    position = oldCamera;
    this->defaultCamera->setPosition(position);
}

void Player::handleInput(CoreInput *input) {
    // Put these up here to avoid CS2360.
    Vector2 delta;
    Vector3 moveVector;

    Vector2 curMousePos = input->getMousePosition();
    delta = ToolBox::subtract(curMousePos, this->oldMousePos);
    //delta = input->getMouseDelta();
    this->oldMousePos = curMousePos;

    if (ToolBox::abs(delta.x) > 300.0f)
        return;
    else if (ToolBox::abs(delta.y) > 300.0f)
        return;

    moveVector = Vector3(0,0,0);
    if (input->getKeyState(KEY_w))
    {
        moveVector += Vector3(0, 0, -1);
    }
    else if (input->getKeyState(KEY_s))
    {
        moveVector += Vector3(0, 0, 1);
    }

    if (input->getKeyState(KEY_d))
    {
        moveVector += Vector3(1, 0, 0);
    }
    else if (input->getKeyState(KEY_a))
    {
        moveVector += Vector3(-1, 0, 0);
    }
    
    
    this->addToCameraPosition(&moveVector);
    this->rotateCamera(&delta, 1.0f);
}

Vector2 Player::getRotation() {
    return Vector2(leftRightRot, upDownRot);
}