#pragma once

#include <Urho3D/Scene/LogicComponent.h>
#include <set>

using namespace Urho3D;

class AnimationController : public LogicComponent {

URHO3D_OBJECT(AnimationController, LogicComponent)

public:
    void static RegisterObject(Context *context);

    AnimationController(Context *context);

    virtual void Start() override;

private:
    void OnRunStart(StringHash eventType, VariantMap &eventData);
    void OnRunStop(StringHash eventType, VariantMap &eventData);
    void OnJumpStart(StringHash eventType, VariantMap &eventData);
    void OnJumpStop(StringHash eventType, VariantMap &eventData);

    enum Animation {
        Idle, Run, Jump
    };

    std::set<Animation> animations;
    Animation current;

    void refresh();
    void pushAnimation(const Animation &animation);
    void pullAnimation(const Animation &animation);
public:
    virtual void FixedPostUpdate(float timeStep) override;
};




