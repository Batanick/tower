#pragma once

#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class AnimationController : public LogicComponent {

URHO3D_OBJECT(AnimationController, LogicComponent)

public:
    void static RegisterObject(Context *context);

    AnimationController(Context *context) : LogicComponent(context) {
        // nothing
    }

    virtual void FixedUpdate(float timeStep) override;
private:
};




