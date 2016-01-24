#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>

#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>

#include "Tower.h"

TowerApp::TowerApp(Context *context) :
        Application(context),
        yaw(0.0f),
        pitch(0.0f),
        screenJoystickIndex(M_MAX_UNSIGNED),
        screenJoystickSettingsIndex(M_MAX_UNSIGNED),
        paused(false),
        drawDebugGeometry(false) {
    // nothing
}

void TowerApp::Setup() {
    engineParameters_["WindowTitle"] = GetTypeName();
    engineParameters_["LogName"] =
            GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs") + GetTypeName() + ".log";
    engineParameters_["FullScreen"] = false;
//    engineParameters_["VSync"] = true;
    engineParameters_["TripleBuffer"] = true;
    engineParameters_["Headless"] = false;
    engineParameters_["Sound"] = false;
}

void TowerApp::Start() {
    // Set custom window Title & Icon
    SetupWindow();

    // Create console and debug HUD
    CreateConsoleAndDebugHud();

    // Subscribe key down event
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(TowerApp, HandleKeyDown));

    // Create the scene content
    CreateScene();

    // Create the UI content
    CreateInstructions();

    // Setup the viewport for displaying the scene
    SetupViewport();

    // Hook up to the frame update events
    SubscribeToEvents();
}

void TowerApp::SetupWindow() {
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    Graphics *graphics = GetSubsystem<Graphics>();
    Image *icon = cache->GetResource<Image>("Textures/UrhoIcon.png");
    graphics->SetWindowIcon(icon);
    graphics->SetWindowTitle("The Tower");
}

void TowerApp::CreateConsoleAndDebugHud() {
    // Get default style
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    XMLFile *xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    // Create console
    Console *console = engine_->CreateConsole();
    console->SetDefaultStyle(xmlFile);
    console->GetBackground()->SetOpacity(0.8f);

    // Create debug HUD.
    DebugHud *debugHud = engine_->CreateDebugHud();
    debugHud->SetDefaultStyle(xmlFile);
}

void TowerApp::CreateScene() {
    scene = new GameScene();
    scene->Init(context_);
}

void TowerApp::CreateInstructions() {
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    UI *ui = GetSubsystem<UI>();

    // Construct new Text object, set string to display and font to use
    Text *instructionText = ui->GetRoot()->CreateChild<Text>();
    instructionText->SetText("Use WASD keys to move, use PageUp PageDown keys to zoom.");
    instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);

    // Position the text relative to the screen center
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_CENTER);
    instructionText->SetPosition(0, ui->GetRoot()->GetHeight() / 4);
}

void TowerApp::SetupViewport() {
    Renderer *renderer = GetSubsystem<Renderer>();

    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, scene->GetScene(), scene->GetCamera()->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
}

void TowerApp::MoveCamera(float timeStep) {
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
        return;

    Input *input = GetSubsystem<Input>();

    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    const SharedPtr<Node> &cameraNode = scene->GetCamera();

    cameraNode->GetComponent<Camera>();
    if (input->GetKeyDown(KEY_PAGEUP)) {
        Camera *camera = cameraNode->GetComponent<Camera>();
        camera->SetZoom(camera->GetZoom() * 1.01f);
    }

    if (input->GetKeyDown(KEY_PAGEDOWN)) {
        Camera *camera = cameraNode->GetComponent<Camera>();
        camera->SetZoom(camera->GetZoom() * 0.99f);
    }
}

void TowerApp::SubscribeToEvents() {
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(TowerApp, HandleUpdate));

    // Unsubscribe the SceneUpdate event from base class to prevent camera pitch and yaw in 2D sample
    UnsubscribeFromEvent(E_SCENEUPDATE);

    // Post render stuff subscription
    SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(TowerApp, HandlePostRenderUpdate));
}

void TowerApp::HandleUpdate(StringHash, VariantMap &eventData) {
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
}

void TowerApp::HandleKeyDown(StringHash, VariantMap &eventData) {
    using namespace KeyDown;

    int key = eventData[P_KEY].GetInt();

    // Close console (if open) or exit when ESC is pressed
    if (key == KEY_ESC) {
        Console *console = GetSubsystem<Console>();
        if (console->IsVisible())
            console->SetVisible(false);
        else
            engine_->Exit();

        return;
    }

    if (key == '`') {
        GetSubsystem<Console>()->Toggle();
        return;
    }

    // Toggle debug HUD with F2
    if (key == KEY_F2) {
        DebugHud *debugHud = GetSubsystem<DebugHud>();
        if (debugHud->GetMode() == 0 || debugHud->GetMode() == DEBUGHUD_SHOW_ALL_MEMORY)
            debugHud->SetMode(DEBUGHUD_SHOW_ALL);
        else
            debugHud->SetMode(DEBUGHUD_SHOW_NONE);
        return;
    }

    if (key == KEY_F3) {
        DebugHud *debugHud = GetSubsystem<DebugHud>();
        if (debugHud->GetMode() == 0 || debugHud->GetMode() == DEBUGHUD_SHOW_ALL)
            debugHud->SetMode(DEBUGHUD_SHOW_ALL_MEMORY);
        else
            debugHud->SetMode(DEBUGHUD_SHOW_NONE);

        return;
    }

    if (key == KEY_F4) {
        drawDebugGeometry = !drawDebugGeometry;
        return;
    }
}

void TowerApp::HandlePostRenderUpdate(StringHash eventType, VariantMap &eventData) {
    PhysicsWorld2D *physicsWorld = scene->GetScene()->GetComponent<PhysicsWorld2D>();
    if (physicsWorld && drawDebugGeometry)
        physicsWorld->DrawDebugGeometry();
}


