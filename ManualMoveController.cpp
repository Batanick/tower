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
    const auto &moveSpeed = GetNode()->GetVar(PROP_SPEED).GetFloat();
    const auto pRigidBody2D = GetNode()->GetComponent<RigidBody2D>();

    if (input->GetKeyPress(KEY_SPACE) && canJump()) {
        const auto &jumpSpeed = GetNode()->GetVar(PROP_JUMP_SPEED).GetFloat();
        pRigidBody2D->ApplyLinearImpulse(Vector2::UP * jumpSpeed, pRigidBody2D->GetMassCenter(), true);
    }

    if (input->GetKeyDown('W')) {
    }

    if (input->GetKeyDown('A')) {
        pRigidBody2D->ApplyLinearImpulse(Vector2::LEFT * moveSpeed * timeStep, pRigidBody2D->GetMassCenter(), true);
    }

    if (input->GetKeyDown('D')) {
        pRigidBody2D->ApplyLinearImpulse(Vector2::RIGHT * moveSpeed * timeStep, pRigidBody2D->GetMassCenter(), true);
    }

    if (input->GetKeyDown('S')) {
    }

}

bool ManualMoveController::canJump() {
    const auto pRigidBody2D = GetNode()->GetComponent<RigidBody2D>();
    return (pRigidBody2D->GetLinearVelocity().y_ == 0.0f); // fine enough
}

void ManualMoveController::RegisterObject(Context *context) {
    context->RegisterFactory<ManualMoveController>();
}
