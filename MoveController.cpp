//
// Created by botanick on 18.01.16.
//

#include <Urho3D/Input/Input.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Urho2D/PhysicsEvents2D.h>

#include "MoveController.h"
#include "Properties.h"

void MoveController::Start() {
    SubscribeToEvent(node_, EVENT_DO_JUMP, URHO3D_HANDLER(MoveController, DoJump));
    SubscribeToEvent(node_, EVENT_DO_MOVE, URHO3D_HANDLER(MoveController, DoMove));
}

void MoveController::DelayedStart() {
    SetUpJumpSensor();
}

void MoveController::RegisterObject(Context *context) {
    context->RegisterFactory<MoveController>();
}

void MoveController::OnStartRunning() {
    node_->SetVar(PROP_IS_RUNNING, true);
    node_->SendEvent(EVENT_RUN_START);
}

void MoveController::OnStopRunning() {
    node_->SetVar(PROP_IS_RUNNING, false);
    node_->SendEvent(EVENT_RUN_STOP);
}

void MoveController::OnJumpStop() {
    node_->SetVar(PROP_IS_JUMPING, false);
    node_->SendEvent(EVENT_JUMP_STOP);
}

void MoveController::OnJumpStart() {
    node_->SetVar(PROP_IS_JUMPING, true);
    node_->SendEvent(EVENT_JUMP_START);
}

void MoveController::DoMove(StringHash eventType, VariantMap &eventData) {
    direction = eventData[EventMove::P_DIRECTION].GetVector2();
}

void MoveController::BeginSense(StringHash eventType, VariantMap &eventData) {
    const auto obj1 = dynamic_cast<Node *>(eventData[PhysicsBeginContact2D::P_NODEA].GetPtr());
    const auto obj2 = dynamic_cast<Node *>(eventData[PhysicsBeginContact2D::P_NODEB].GetPtr());

    if (obj1 == sensor || obj2 == sensor) {
        sensedCount++;
    }
}

void MoveController::StopSense(StringHash eventType, VariantMap &eventData) {
    const auto obj1 = dynamic_cast<Node *>(eventData[PhysicsBeginContact2D::P_NODEA].GetPtr());
    const auto obj2 = dynamic_cast<Node *>(eventData[PhysicsBeginContact2D::P_NODEB].GetPtr());

    if (obj1 == sensor || obj2 == sensor) {
        sensedCount--;
    }
}

void MoveController::DoJump(StringHash eventType, VariantMap &eventData) {
    if (node_->GetVar(PROP_IS_JUMPING).GetBool()) {
        return;
    }

    wantJump = true;
}

void MoveController::FixedUpdate(float timeStep) {
    const auto &pRigidBody2D = node_->GetComponent<RigidBody2D>();

    const bool running = node_->GetVar(PROP_IS_RUNNING).GetBool();
    if (direction != Vector2::ZERO) {
        const auto &moveSpeed = node_->GetVar(PROP_SPEED).GetFloat();

        pRigidBody2D->ApplyLinearImpulse(direction.Normalized() * moveSpeed * timeStep, pRigidBody2D->GetMassCenter(),
                                         true);
        node_->SetVar(PROP_DIRECTION, direction);

        if (!running) {
            OnStartRunning();
        }
    } else if (running) {
        OnStopRunning();
    }

    const auto jumping = node_->GetVar(PROP_IS_JUMPING).GetBool();
    if (!IsFlying()) {
        if (jumping) {
            OnJumpStop();
        }

        if (wantJump) {
            const auto &jumpSpeed = node_->GetVar(PROP_JUMP_SPEED).GetFloat();
            pRigidBody2D->ApplyLinearImpulse(Vector2::UP * jumpSpeed, pRigidBody2D->GetMassCenter(), true);
            OnJumpStart();
        }

    }

    direction = Vector2::ZERO;
    wantJump = false;
}

void MoveController::SetUpJumpSensor() {
    sensor = node_->CreateChild("jump_sensor");
    const auto collision = node_->GetDerivedComponent<CollisionBox2D>();
    sensor->SetPosition2D(0.0, -collision->GetSize().y_ / 2);

    const auto rigidBody = sensor->CreateComponent<RigidBody2D>();
    rigidBody->SetAllowSleep(false);
    rigidBody->SetBodyType(BT_DYNAMIC);
    rigidBody->SetMass(0.0);
    rigidBody->SetGravityScale(0.0f);
    rigidBody->SetVolatilePosition(true);

    CollisionBox2D *box = sensor->CreateComponent<CollisionBox2D>();
    box->SetTrigger(true);
    box->SetGroupIndex(-((short) node_->GetID()));
    box->SetSize(collision->GetSize().x_ * 0.7f, 0.1f);

    sensor->SubscribeToEvent(E_PHYSICSBEGINCONTACT2D, URHO3D_HANDLER(MoveController, BeginSense));
    sensor->SubscribeToEvent(E_PHYSICSENDCONTACT2D, URHO3D_HANDLER(MoveController, StopSense));
}
