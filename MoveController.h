#pragma once

//
// Created by botanick on 18.01.16.
//

#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class MoveController : public LogicComponent {

URHO3D_OBJECT(MoveController, LogicComponent)

public:
    void static RegisterObject(Context *context);

    MoveController(Context *context)
            : LogicComponent(context),
              direction(Vector2::ZERO),
              wantJump(false), sensedCount(0) {
        SetUpdateEventMask(USE_FIXEDUPDATE);
    }

private:
    void OnStartRunning();
    void OnStopRunning();
    void OnJumpStop();
    void OnJumpStart();

    void DoJump(StringHash eventType, VariantMap &eventData);
    void DoMove(StringHash eventType, VariantMap &eventData);

    void BeginSense(StringHash eventType, VariantMap &eventData);
    void StopSense(StringHash eventType, VariantMap &eventData);

    bool IsFlying() {
        return sensedCount <= 0;
    }

    Vector2 direction;
    bool wantJump;

    SharedPtr<Node> sensor;
    int sensedCount;
public:
    virtual void Start() override;
    virtual void FixedUpdate(float timeStep) override;
    void SetUpJumpSensor();
    virtual void DelayedStart() override;
};



