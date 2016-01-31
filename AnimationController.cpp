//
// Created by botanick on 31.01.16.
//

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/IO/Log.h>

#include "AnimationController.h"
#include "Properties.h"

AnimationController::AnimationController(Context *context) : LogicComponent(context) {
    // nothing
    animations.insert(Animation::Idle);
    current = Animation::Idle;
}

void AnimationController::RegisterObject(Context *context) {
    context->RegisterFactory<AnimationController>();
}

void AnimationController::Start() {
    auto sprite = node_->GetComponent<AnimatedSprite2D>();
    sprite->SetAnimation("idle");

    SubscribeToEvent(node_, EVENT_RUN_START, URHO3D_HANDLER(AnimationController, OnRunStart));
    SubscribeToEvent(node_, EVENT_RUN_STOP, URHO3D_HANDLER(AnimationController, OnRunStop));
    SubscribeToEvent(node_, EVENT_JUMP_START, URHO3D_HANDLER(AnimationController, OnJumpStart));
    SubscribeToEvent(node_, EVENT_JUMP_STOP, URHO3D_HANDLER(AnimationController, OnJumpStop));
}

void AnimationController::OnRunStart(StringHash eventType, VariantMap &eventData) {
    pushAnimation(Animation::Run);
}

void AnimationController::OnJumpStart(StringHash eventType, VariantMap &eventData) {
    pushAnimation(Animation::Jump);
}

void AnimationController::OnRunStop(StringHash eventType, VariantMap &eventData) {
    pullAnimation(Animation::Run);
}

void AnimationController::OnJumpStop(StringHash eventType, VariantMap &eventData) {
    pullAnimation(Animation::Jump);
}

void AnimationController::FixedPostUpdate(float timeStep) {
    const auto direction = node_->GetVar(PROP_DIRECTION).GetVector2();
    auto sprite = node_->GetComponent<AnimatedSprite2D>();
    sprite->SetFlipX(direction == Vector2::LEFT);
}

void AnimationController::pullAnimation(const AnimationController::Animation &animation) {
    animations.erase(animation);
    refresh();
}

void AnimationController::pushAnimation(const AnimationController::Animation &animation) {
    animations.insert(animation);
    refresh();
}

void AnimationController::refresh() {
    auto sprite = node_->GetComponent<AnimatedSprite2D>();
    if (animations.empty()) {
        sprite->SetAnimation("idle");
        current = Animation::Idle;
    }

    auto it = animations.rbegin();
    if (*it != current) {
        current = *it;

        switch (current) {
            case Animation::Idle:
                sprite->SetAnimation("idle");
                break;
            case Animation::Jump:
                sprite->SetAnimation("jump_start", LoopMode2D::LM_FORCE_LOOPED);
                break;
            case Animation::Run:
                sprite->SetAnimation("run");
                break;
            case Animation::Shoot:
                sprite->SetAnimation("shoot");
                break;
        }
    }
}
