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

#include "Tower.h"

TowerApp::TowerApp(Context *context) :
        Application(context),
        yaw_(0.0f),
        pitch_(0.0f),
        screenJoystickIndex_(M_MAX_UNSIGNED),
        screenJoystickSettingsIndex_(M_MAX_UNSIGNED),
        paused_(false) {
}

void TowerApp::Setup() {
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

    graphics->SetMode(800, 600, false, true, false, true, false, false);
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

    // Movement speed as world units per second
    const float MOVE_SPEED = 4.0f;

    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    const SharedPtr<Node> &cameraNode = scene->GetCamera();
    if (input->GetKeyDown('W')) {
        cameraNode->Translate(Vector3::UP * MOVE_SPEED * timeStep);
    }
    if (input->GetKeyDown('S'))
        cameraNode->Translate(Vector3::DOWN * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('A'))
        cameraNode->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('D'))
        cameraNode->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);

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
    }

        // Toggle console with F1
    else if (key == KEY_TAB)
        GetSubsystem<Console>()->Toggle();

        // Toggle debug HUD with F2
    else if (key == KEY_F2) {
        DebugHud *debugHud = GetSubsystem<DebugHud>();
        if (debugHud->GetMode() == 0 || debugHud->GetMode() == DEBUGHUD_SHOW_ALL_MEMORY)
            debugHud->SetMode(DEBUGHUD_SHOW_ALL);
        else
            debugHud->SetMode(DEBUGHUD_SHOW_NONE);
    }
    else if (key == KEY_F3) {
        DebugHud *debugHud = GetSubsystem<DebugHud>();
        if (debugHud->GetMode() == 0 || debugHud->GetMode() == DEBUGHUD_SHOW_ALL)
            debugHud->SetMode(DEBUGHUD_SHOW_ALL_MEMORY);
        else
            debugHud->SetMode(DEBUGHUD_SHOW_NONE);
    }

        // Common rendering quality controls, only when UI has no focused element
    else if (!GetSubsystem<UI>()->GetFocusElement()) {
        Renderer *renderer = GetSubsystem<Renderer>();

        if (key == '1') {
            int quality = renderer->GetTextureQuality();
            ++quality;
            if (quality > QUALITY_HIGH)
                quality = QUALITY_LOW;
            renderer->SetTextureQuality(quality);
        }

            // Material quality
        else if (key == '2') {
            int quality = renderer->GetMaterialQuality();
            ++quality;
            if (quality > QUALITY_HIGH)
                quality = QUALITY_LOW;
            renderer->SetMaterialQuality(quality);
        }

            // Specular lighting
        else if (key == '3')
            renderer->SetSpecularLighting(!renderer->GetSpecularLighting());

            // Shadow rendering
        else if (key == '4')
            renderer->SetDrawShadows(!renderer->GetDrawShadows());

            // Shadow map resolution
        else if (key == '5') {
            int shadowMapSize = renderer->GetShadowMapSize();
            shadowMapSize *= 2;
            if (shadowMapSize > 2048)
                shadowMapSize = 512;
            renderer->SetShadowMapSize(shadowMapSize);
        }

            // Shadow depth and filtering quality
        else if (key == '6') {
            int quality = renderer->GetShadowQuality();
            ++quality;
            if (quality > SHADOWQUALITY_HIGH_24BIT)
                quality = SHADOWQUALITY_LOW_16BIT;
            renderer->SetShadowQuality(quality);
        }

            // Occlusion culling
        else if (key == '7') {
            bool occlusion = renderer->GetMaxOccluderTriangles() > 0;
            occlusion = !occlusion;
            renderer->SetMaxOccluderTriangles(occlusion ? 5000 : 0);
        }

            // Instancing
        else if (key == '8')
            renderer->SetDynamicInstancing(!renderer->GetDynamicInstancing());

            // Take screenshot
        else if (key == '9') {
            Graphics *graphics = GetSubsystem<Graphics>();
            Image screenshot(context_);
            graphics->TakeScreenShot(screenshot);
            // Here we save in the Data folder with date and time appended
            screenshot.SavePNG(GetSubsystem<FileSystem>()->GetProgramDir() + "Data/Screenshot_" +
                               Time::GetTimeStamp().Replaced(':', '_').Replaced('.', '_').Replaced(' ', '_') + ".png");
        }
    }
}

