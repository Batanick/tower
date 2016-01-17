#pragma once

#include <Urho3D/Engine/Application.h>

namespace Urho3D {
    class Node;
    class Scene;
    class Sprite;
}

using namespace Urho3D;

class TowerApp : public Application {

URHO3D_OBJECT(TowerApp, Application);

public:
    TowerApp(Context *context);

    virtual void Setup() override;
    virtual void Start() override;

protected:
    /// Return XML patch instructions for screen joystick layout for a specific sample app, if any.
    virtual String GetScreenJoystickPatchString() const {
        return
                "<patch>"
                        "    <remove sel=\"/element/element[./attribute[@name='Name' and @value='Button0']]/attribute[@name='Is Visible']\" />"
                        "    <replace sel=\"/element/element[./attribute[@name='Name' and @value='Button0']]/element[./attribute[@name='Name' and @value='Label']]/attribute[@name='Text']/@value\">Zoom In</replace>"
                        "    <add sel=\"/element/element[./attribute[@name='Name' and @value='Button0']]\">"
                        "        <element type=\"Text\">"
                        "            <attribute name=\"Name\" value=\"KeyBinding\" />"
                        "            <attribute name=\"Text\" value=\"PAGEUP\" />"
                        "        </element>"
                        "    </add>"
                        "    <remove sel=\"/element/element[./attribute[@name='Name' and @value='Button1']]/attribute[@name='Is Visible']\" />"
                        "    <replace sel=\"/element/element[./attribute[@name='Name' and @value='Button1']]/element[./attribute[@name='Name' and @value='Label']]/attribute[@name='Text']/@value\">Zoom Out</replace>"
                        "    <add sel=\"/element/element[./attribute[@name='Name' and @value='Button1']]\">"
                        "        <element type=\"Text\">"
                        "            <attribute name=\"Name\" value=\"KeyBinding\" />"
                        "            <attribute name=\"Text\" value=\"PAGEDOWN\" />"
                        "        </element>"
                        "    </add>"
                        "</patch>";
    }

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
    SharedPtr<Scene> scene_;
    /// Camera scene node.
    SharedPtr<Node> cameraNode_;
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
