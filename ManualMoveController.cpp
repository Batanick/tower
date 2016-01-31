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
    const auto &pRigidBody2D = node_->GetComponent<RigidBody2D>();

    const bool running = node_->GetVar(PROP_IS_RUNNING).GetBool();
    const bool jumping = node_->GetVar(PROP_IS_JUMPING).GetBool();

    const bool commandLeft = input->GetKeyDown('A');
    const bool commandRight = input->GetKeyDown('D');
    if (commandLeft ^ commandRight) {
        auto direction = commandLeft ? Vector2::LEFT : Vector2::RIGHT;
        const auto &moveSpeed = node_->GetVar(PROP_SPEED).GetFloat();
        pRigidBody2D->ApplyLinearImpulse(direction * moveSpeed * timeStep, pRigidBody2D->GetMassCenter(), true);
        node_->SetVar(PROP_DIRECTION, direction);

        if (!running) {
            OnStartRunning();
        }
    } else if (running) {
        OnStopRunning();
    }

    if (!Flying()) {
        if (jumping) {
            OnJumpStop();
        }

        if (input->GetKeyPress(KEY_SPACE)) {
            const auto &jumpSpeed = node_->GetVar(PROP_JUMP_SPEED).GetFloat();
            pRigidBody2D->ApplyLinearImpulse(Vector2::UP * jumpSpeed, pRigidBody2D->GetMassCenter(), true);

            OnJumpStart();
        }
    }
}

bool ManualMoveController::Flying() {
    const auto pRigidBody2D = node_->GetComponent<RigidBody2D>();
    // dirty hack, but fine enough for now ^_^
    return (std::fabs(pRigidBody2D->GetLinearVelocity().y_) >= 0.00001f);
}

void ManualMoveController::RegisterObject(Context *context) {
    context->RegisterFactory<ManualMoveController>();
}

void ManualMoveController::OnStartRunning() {
    node_->SetVar(PROP_IS_RUNNING, true);
    node_->SendEvent(EVENT_RUN_START);
}

void ManualMoveController::OnStopRunning() {
    node_->SetVar(PROP_IS_RUNNING, false);
    node_->SendEvent(EVENT_RUN_STOP);
}

void ManualMoveController::OnJumpStop() {
    node_->SetVar(PROP_IS_JUMPING, false);
    node_->SendEvent(EVENT_JUMP_STOP);
}

void ManualMoveController::OnJumpStart() {
    node_->SetVar(PROP_IS_JUMPING, true);
    node_->SendEvent(EVENT_JUMP_START);
}
