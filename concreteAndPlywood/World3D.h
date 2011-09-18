#pragma once
#include "Polycode.h"
#include "PolycodeView.h"
#include "Player.h"

using namespace Polycode;
using namespace PlayerSpace;
using namespace ToolBox;

class World3D : public EventHandler {
public:
    World3D(PolycodeView *view);
    ~World3D();

    bool update();
private:
    void setupWorld();
    void handleEvent(Event *e);
    void handleInput(InputEvent *e);

    Core *core;
    Scene *scene;
    Screen *screen;
    
    SceneLight *flashLight, *pLight;
    ScreenLabel *rotationText, *rotationTextDos, *fps;
    ScenePrimitive *ground, *box;
    Player *mPlayer;
};