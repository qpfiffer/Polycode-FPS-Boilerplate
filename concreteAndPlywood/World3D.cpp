#include "World3D.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace Polycode;

World3D::World3D(PolycodeView *view) {
    this->view = view;
    core = new Win32Core(view, 1280, 720, false, 2, 60);
    //core = new Win32Core(view, 1680, 1050, true, 2, 60);
    CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
    CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
    CoreServices::getInstance()->getResourceManager()->addDirResource("assets", false);

    // Take an initial snapshot of the mouse state:
    SetCursorPos(core->getXRes()/2, core->getYRes()/2);
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    mouseState.x = (int)cursorPos.x;
    mouseState.y = (int)cursorPos.y;
    //core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);
    core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEMOVE);
    //core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEUP);
    core->getInput()->addEventListener(this, InputEvent::EVENT_KEYDOWN);
    core->enableMouse(false);

    scene = new Scene();
    scene->getDefaultCamera()->setFOV(75.0);
    scene->enableFog(true);
    Color fogColor = Color(224, 255, 255, 255);
    scene->setFogProperties(Renderer::FOG_EXP, fogColor, 0.3, 15.0, 80.0);

    prims = new list<ScenePrimitive *>;
    meshes = new list<SceneMesh *>;

    mPlayer = new Player(scene->getDefaultCamera());

    screen = new Screen();
    rotationText = new ScreenLabel("NOTHING", 12);
    rotationText->setColor(255,0,0,255);
    rotationTextDos = new ScreenLabel("NOTHING", 12);
    rotationTextDos->setPosition(0, 15, 0);
    rotationTextDos->setColor(255,0,0,255);
    fps = new ScreenLabel("NOTHING", 12);
    fps->setColor(255,0,0,255);
    fps->setPosition(0, 30, 0);
    deltaText = new ScreenLabel("NOTHING", 12);
    deltaText->setColor(255,0,0,255);
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
    delete flashLight;
    delete pLight;
    delete mPlayer;
    delete ambientSound;

    // Text:
    delete rotationText;
    delete rotationTextDos;
    delete fps;
    delete deltaText;
    delete screen;

    // Collections of items:
    this->prims->clear();
    this->meshes->clear();
    delete prims;
    delete meshes;

    // Man items
    delete scene;
    delete core;
}

bool World3D::update() {
    Vector2 oldDelta = core->getInput()->getMouseDelta();

    if (oldDelta.x != 0 || oldDelta.y != 0 || oldDelta.z !=0) {
        core->getInput()->getMouseDelta();
    }

    std::stringstream ss;
    ss << "Left right: " << -this->mPlayer->getRotation().x;
    this->rotationText->setText(ss.str());

    ss.str("");
    ss << "Up down: " << -this->mPlayer->getRotation().y;
    this->rotationTextDos->setText(ss.str());

    ss.str("");
    ss << "FPS: " << core->getFPS();
    this->fps->setText(ss.str());

    this->mPlayer->pollKeyboardInput(core->getInput());

    return core->Update();
}

void World3D::setupWorld() {

    /*for (int i = 0; i<4; i++) {
        for (int j = 0; j<4; j++) {
            ScenePrimitive *groundTile = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, 5,5);
            groundTile->setMaterialByName("GroundMaterial");
            groundTile->setPosition(i*5, 0, j*5);
            scene->addEntity(groundTile);

            prims->push_back(groundTile);
        }
    }*/
    for (int i = 0; i<15; i++) {
        const Number boxSize = 1;
        const Number railingScale = 0.3;
        ScenePrimitive *raisedPlat = new ScenePrimitive(ScenePrimitive::TYPE_BOX, boxSize,0.3,boxSize);
        raisedPlat->setMaterialByName("CubeMaterial");
        raisedPlat->setPosition(0, 0, i*boxSize);
        scene->addEntity(raisedPlat);

        prims->push_back(raisedPlat);

        // Left railing:
        SceneMesh *railingMesh = new SceneMesh("assets/railing.mesh");
        railingMesh->loadTexture("railing.png");
        railingMesh->Scale(railingScale, railingScale, railingScale);
        railingMesh->setPosition((boxSize/2),0,i*boxSize);
        scene->addEntity(railingMesh);

        meshes->push_back(railingMesh);

        // Right railing:
        railingMesh = new SceneMesh("assets/railing.mesh");
        railingMesh->loadTexture("railing.png");
        railingMesh->Scale(railingScale, railingScale, railingScale);
        railingMesh->setPosition(-(boxSize/2),0,i*boxSize);
        scene->addEntity(railingMesh);

        meshes->push_back(railingMesh);
    }
    ScenePrimitive *ground = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, 60, 60);
    ground->setMaterialByName("GroundMaterial");
    scene->addEntity(ground);
    prims->push_back(ground);
    
    /*box->setMaterialByName("CubeMaterial");
    scene->addEntity(box);*/

    pLight->setPosition(0,3,0);
    scene->addLight(pLight);

    ambientSound = new Sound("assets/ambient.wav");
    ambientSound->Play(true);

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
    //HWND hWnd;
    //HWND fakeWnd;
    //LPCTSTR fakeTitle = L"FAKE";
    Vector2 mousePosition = e->getMousePosition();
    switch(e->getEventCode()) {
    case InputEvent::EVENT_MOUSEMOVE:        
        if (mousePosition.x != 637 &&
            mousePosition.y != 335) {
            this->mPlayer->handleMouseInput(e);
            SetCursorPos(core->getXRes()/2, core->getYRes()/2);
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            mouseState.x = (int)cursorPos.x;
            mouseState.y = (int)cursorPos.y;
            std::stringstream ss;
            ss << "MouseState: " << this->mouseState.x << " " << this->mouseState.y << " MousePosition: " <<
                mousePosition.x << " " << mousePosition.y;
            this->deltaText->setText(ss.str());
        }
        /*hWnd = GetCapture();
        fakeWnd = GetDesktopWindow();
        ReleaseCapture();
        SetCapture(fakeWnd);
        SetCursorPos(core->getXRes()/2, core->getYRes()/2);
        ReleaseCapture();
        SetCapture(hWnd);*/
        break;
    case InputEvent::EVENT_KEYDOWN:
        if (e->key == KEY_ESCAPE) {
            this->core->Shutdown();
        }
        break;
    }
}