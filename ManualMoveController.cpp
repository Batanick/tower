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

    if (input->GetKeyDown('W')) {
        pRigidBody2D->ApplyLinearImpulse(Vector2::UP * moveSpeed * timeStep, pRigidBody2D->GetMassCenter(), true);
    }

//        GetNode()->(Vector3::UP * moveSpeed * timeStep);
    if (input->GetKeyDown('S')) {

    }
//        GetNode()->Translate(Vector3::DOWN * moveSpeed * timeStep);
    if (input->GetKeyDown('A')) {
        pRigidBody2D->ApplyLinearImpulse(Vector2::LEFT * moveSpeed * timeStep, pRigidBody2D->GetMassCenter(), true);

    }
//        GetNode()->Translate(Vector3::LEFT * moveSpeed * timeStep);
    if (input->GetKeyDown('D')) {
        pRigidBody2D->ApplyLinearImpulse(Vector2::RIGHT * moveSpeed * timeStep, pRigidBody2D->GetMassCenter(), true);
    }
//        GetNode()->Translate(Vector3::RIGHT * moveSpeed * timeStep);
}

void ManualMoveController::RegisterObject(Context *context) {
    context->RegisterFactory<ManualMoveController>();
}
