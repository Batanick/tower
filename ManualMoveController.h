#pragma once

//
// Created by botanick on 18.01.16.
//

#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class ManualMoveController : public LogicComponent {

URHO3D_OBJECT(ManualMoveController, LogicComponent)

public:
    void static RegisterObject(Context *context);

    ManualMoveController(Context *context)
            : LogicComponent(context) {
        SetUpdateEventMask(USE_FIXEDUPDATE);
    }

    virtual void FixedUpdate(float timeStep) override;

private:
    bool Flying();

    void OnStartRunning();
    void OnStopRunning();
    void OnJumpStop();
    void OnJumpStart();
};



