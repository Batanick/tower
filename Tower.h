#pragma once

#include <Urho3D/Engine/Application.h>
#include "GameScene.h"

using namespace Urho3D;

class TowerApp : public Application {

URHO3D_OBJECT(TowerApp, Application);

public:
    TowerApp(Context *context);

    virtual void Setup() override;
    virtual void Start() override;

protected:

private:
    /// Construct the scene content.
    void CreateScene();
    /// Construct an instruction text to the UI.
    void CreateInstructions();
    /// Set up a viewport for displaying the scene.
    void SetupViewport();
    /// Read input and moves the camera.
    void MoveCamera(float timeStep);
    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap &eventData);
    void SetupWindow();
    void CreateConsoleAndDebugHud();
    void HandleKeyDown(StringHash eventType, VariantMap &eventData);

    /// Scene.
    SharedPtr<GameScene> scene;

    /// Camera yaw angle.
    float yaw_;
    /// Camera pitch angle.
    float pitch_;
    /// Screen joystick index for navigational controls (mobile platforms only).
    int screenJoystickIndex_;
    /// Screen joystick index for settings (mobile platforms only).
    int screenJoystickSettingsIndex_;
    /// Pause flag.
    bool paused_;
};
