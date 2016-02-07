//
// Created by botanick on 31.01.16.
//
#include <Urho3D/Input/Input.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Core/Context.h>

#include "Legat.h"

#include "Properties.h"

void Legat::RegisterObject(Context *context) {
    context->RegisterFactory<Legat>();
}

void Legat::FixedUpdate(float timeStep) {
    const auto &input = GetSubsystem<Input>();

    const bool commandLeft = input->GetKeyDown('A');
    const bool commandRight = input->GetKeyDown('D');

    if (commandLeft ^ commandRight) {
        VariantMap params;
        params[EventMove::P_DIRECTION] = commandLeft ? Vector2::LEFT : Vector2::RIGHT;
        node_->SendEvent(EVENT_DO_MOVE, params);
    }

    if (input->GetKeyPress(KEY_SPACE)) {
        node_->SendEvent(EVENT_DO_JUMP);
    }
}
