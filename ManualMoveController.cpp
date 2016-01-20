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

    if (input->GetKeyDown('W'))
        GetNode()->Translate(Vector3::UP * moveSpeed * timeStep);
    if (input->GetKeyDown('S'))
        GetNode()->Translate(Vector3::DOWN * moveSpeed * timeStep);
    if (input->GetKeyDown('A'))
        GetNode()->Translate(Vector3::LEFT * moveSpeed * timeStep);
    if (input->GetKeyDown('D'))
        GetNode()->Translate(Vector3::RIGHT * moveSpeed * timeStep);

    if (input->GetKeyDown(KEY_SPACE)) {
        const auto pRigidBody2D = GetNode()->GetComponent<RigidBody2D>();
        if (pRigidBody2D)
            pRigidBody2D->ApplyForceToCenter(Vector2(0.0, 1.0), true);
    }
}

void ManualMoveController::RegisterObject(Context *context) {
    context->RegisterFactory<ManualMoveController>();
}
