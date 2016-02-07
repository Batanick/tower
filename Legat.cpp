//
// Created by botanick on 31.01.16.
//
#include <Urho3D/Input/Input.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Graphics.h>

#include "Legat.h"

#include "Properties.h"

void Legat::RegisterObject(Context *context) {
    context->RegisterFactory<Legat>();
}

void Legat::DelayedStart() {
    SubscribeToEvent(E_UIMOUSECLICK, URHO3D_HANDLER(Legat, OnMouseDown));
}

void Legat::FixedUpdate(float timeStep) {
    const auto &input = GetSubsystem<Input>();

    const bool commandLeft = input->GetKeyDown('A');
    const bool commandRight = input->GetKeyDown('D');

    if (commandLeft ^ commandRight) {
        VariantMap params;
        params[EventDoMove::P_DIRECTION] = commandLeft ? Vector2::LEFT : Vector2::RIGHT;
        node_->SendEvent(EVENT_DO_MOVE, params);
    }

    if (input->GetKeyPress(KEY_SPACE)) {
        node_->SendEvent(EVENT_DO_JUMP);
    }
}

void Legat::OnMouseDown(StringHash eventType, VariantMap &eventData) {
    const auto cameraNode = GetScene()->GetChild(NAME_CAMERA);
    if (!cameraNode) {
        return;
    }

    const auto pCamera = cameraNode->GetComponent<Camera>();
    if (!pCamera) {
        return;
    }

    const auto graphics = GetSubsystem<Graphics>();

    float x = (float) eventData[UIMouseClick::P_X].GetInt();
    float y = (float) eventData[UIMouseClick::P_Y].GetInt();

    auto target = pCamera->ScreenToWorldPoint(Vector3(x / graphics->GetWidth(), y / graphics->GetHeight(), 0.0f));

    VariantMap params;
    params[EventDoShoot::P_TARGET] = Vector2(target.x_, target.y_);
    node_->SendEvent(EVENT_DO_SHOOT, params);
}
