#include "World3D.h"
#include <iostream>
#include <sstream>

using namespace std;

World3D::World3D(PolycodeView *view) {
    core = new Win32Core(view, 1280, 720, false, 2, 60);
    CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
    CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
    CoreServices::getInstance()->getResourceManager()->addDirResource("assets", false);

    //core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);
    //core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEMOVE);
    //core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEUP);
    core->getInput()->addEventListener(this, InputEvent::EVENT_KEYDOWN);
    core->enableMouse(false);

    scene = new Scene();

    ground = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, 60, 60);
    box = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 1, 1, 1);
    mPlayer = new Player(scene->getDefaultCamera());

    screen = new Screen();
    rotationText = new ScreenLabel("NOTHING", 12);
    rotationTextDos = new ScreenLabel("NOTHING", 12);
    rotationTextDos->setPosition(0, 15, 0);
    fps = new ScreenLabel("NOTHING", 12);
    fps->setPosition(0, 30, 0);
    deltaText = new ScreenLabel("NOTHING", 12);
    deltaText->setPosition(0,45,0);

    screen->addChild(rotationText);
    screen->addChild(rotationTextDos);
    screen->addChild(fps);
    screen->addChild(deltaText);

    flashLight = new SceneLight(SceneLight::SPOT_LIGHT, scene, 3);
    pLight = new SceneLight(SceneLight::AREA_LIGHT, scene, 6);

    this->setupWorld();
}

World3D::~World3D() {
    // Items in scene:
    delete ground;
    delete box;
    delete flashLight;
    delete pLight;
    delete mPlayer;

    // Text:
    delete rotationText;
    delete rotationTextDos;
    delete fps;
    delete deltaText;
    delete screen;

    // Man items
    delete scene;
    delete core;
}

bool World3D::update() {
    std::stringstream ss;
    ss << "Left right: " << -this->mPlayer->getRotation().x;
    this->rotationText->setText(ss.str());

    ss.str("");
    ss << "Up down: " << -this->mPlayer->getRotation().y;
    this->rotationTextDos->setText(ss.str());

    ss.str("");
    ss << "FPS: " << core->getFPS();
    this->fps->setText(ss.str());

    this->mPlayer->handleInput(core->getInput(), deltaText);

#ifdef WIN32
    SetCursorPos(core->getXRes(), core->getYRes());
#endif

    return core->Update();
}

void World3D::setupWorld() {
    ground->setMaterialByName("GroundMaterial");
    scene->addEntity(ground);

    box->setMaterialByName("CubeMaterial");
    scene->addEntity(box);

    pLight->setPosition(0,3,0);
    scene->addLight(pLight);

    flashLight->setPosition(7,7,7);
	flashLight->setSpotlightProperties(30,6);
	flashLight->setLightColor(0.96, 1.0, 1.0);
    flashLight->lookAt(Vector3(0,0,0));
	scene->addLight(flashLight);

    scene->getDefaultCamera()->setPosition(0,0.5,0);
    scene->getDefaultCamera()->lookAt(Vector3(0,0,0));
}

void World3D::handleEvent(Event *e) {
    // Handle input:
    if (e->getDispatcher() == core->getInput()) {
        this->handleInput((InputEvent *)e);
    }
    
}

void World3D::handleInput(InputEvent *e) {
    switch(e->getEventCode()) {
        case InputEvent::EVENT_KEYDOWN:
            if (e->key == KEY_ESCAPE) {
                this->core->Shutdown();
            }
            break;
    }
}