//
// Created by botanick on 18.01.16.
//

#include <Urho3D/Input/Input.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Urho2D/RigidBody2D.h>

#include "ManualMoveController.h"
#include "Properties.h"

void ManualMoveController::FixedUpdate(float timeStep) {
    const auto &input = GetSubsystem<Input>();
    const auto &pRigidBody2D = GetNode()->GetComponent<RigidBody2D>();

    bool commandLeft = input->GetKeyDown('A');
    bool commandRight = input->GetKeyDown('D');
    if (commandLeft ^ commandRight) {
        auto direction = commandLeft ? Vector2::LEFT : Vector2::RIGHT;
        const auto &moveSpeed = GetNode()->GetVar(PROP_SPEED).GetFloat();
        pRigidBody2D->ApplyLinearImpulse(direction * moveSpeed * timeStep, pRigidBody2D->GetMassCenter(), true);

        if (!running) {
            running = true;
            onStartRunning();
        }
    } else if (running) {
        running = false;
        onStopRunning();
    }

    if (jumping && flying()) {
        jumping = false;
        onJumpStop();
    }

    if (input->GetKeyPress(KEY_SPACE) && !flying()) {
        const auto &jumpSpeed = GetNode()->GetVar(PROP_JUMP_SPEED).GetFloat();
        pRigidBody2D->ApplyLinearImpulse(Vector2::UP * jumpSpeed, pRigidBody2D->GetMassCenter(), true);

        jumping = true;
        onJumpStart();
    }
}

bool ManualMoveController::flying() {
    const auto pRigidBody2D = GetNode()->GetComponent<RigidBody2D>();
    // dirty hack, but fine enough for now ^_^
    return (std::fabs(pRigidBody2D->GetLinearVelocity().y_) >= 0.01f);
}

void ManualMoveController::RegisterObject(Context *context) {
    context->RegisterFactory<ManualMoveController>();
}

void ManualMoveController::onStartRunning() {
    GetNode()->SendEvent(EVENT_RUN_START);
}

void ManualMoveController::onStopRunning() {
    GetNode()->SendEvent(EVENT_RUN_STOP);
}

void ManualMoveController::onJumpStop() {
    GetNode()->SendEvent(EVENT_JUMP_STOP);
}

void ManualMoveController::onJumpStart() {
    GetNode()->SendEvent(EVENT_JUMP_START);
}
