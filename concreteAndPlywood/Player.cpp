#include "Player.h"
#include <sstream>

using namespace PlayerSpace;
using namespace ToolBox;

Player::Player(Camera *defaultCamera): 
    didRotate(false), position(Vector3(0,0.8,0)), oldMousePos(Vector2(0,0)), 
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
    this->leftRightRot -= rotationSpeed * mouseDifference->x;
    if (leftRightRot > 360.0f)
        leftRightRot -= (float)(360.0f);
    else if (leftRightRot < -(360.0f))
        leftRightRot += (float)(360.0f);

    if (upDownRot - (rotationSpeed * mouseDifference->y) < 90.0f &&
        upDownRot - (rotationSpeed * mouseDifference->y) > -(90.0f))
    {
        upDownRot -= rotationSpeed * mouseDifference->y;
    }

    this->defaultCamera->setYaw(-this->leftRightRot);
    this->defaultCamera->setPitch(-this->upDownRot);
}

void Player::addToCameraPosition(Vector3 *toAdd) {
    // Set the temporary camera position variable:
    Vector3 oldCamera = position;

    Matrix4 cameraRotation;

    cameraRotation = ToolBox::CreateRotationY(ToolBox::ToRadians(leftRightRot)) *
        ToolBox::CreateRotationX(0.0f);

    //Vector3 rotatedVector = Vector3.Transform(vectorToAdd, cameraRotation);
    Vector3 rotatedVector = cameraRotation.rotateVector(*toAdd);
    oldCamera += (rotatedVector * moveSpeed);

    position = oldCamera;
    this->defaultCamera->setPosition(position);
}

void Player::pollKeyboardInput(CoreInput *input) {
    Vector3 moveVector;
    
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
}

void Player::handleMouseInput(InputEvent *e) {
    // Put these up here to avoid CS2360.
    Vector2 delta;

    Vector2 curMousePos = e->getMousePosition();
    //delta = ToolBox::subtract(curMousePos, this->oldMousePos);
#ifdef _DEBUG
    /*stringstream ss;
    ss << "Delta X: " << delta.x << " Delta y: " << delta.y;
    deltaText->setText(ss.str());*/
#endif
    //if (delta.x != 0 && delta.y != 0)
    this->rotateCamera(&delta, 1.0f);

    this->oldMousePos = e->getMousePosition();

    /*if (ToolBox::abs(delta.x) > 300.0f)
        return;
    else if (ToolBox::abs(delta.y) > 300.0f)
        return;*/
}

Vector2 Player::getRotation() {
    return Vector2(leftRightRot, upDownRot);
}