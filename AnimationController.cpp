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
    animations.insert(Animation(AnimationPriority::Idle, true, "idle"));
}

void AnimationController::RegisterObject(Context *context) {
    context->RegisterFactory<AnimationController>();
}

void AnimationController::Start() {
    current = AnimationPriority::Idle;
    timeout = 0.0f;

    auto sprite = node_->GetComponent<AnimatedSprite2D>();
    sprite->SetAnimation("idle");

    SubscribeToEvent(node_->GetParent(), EVENT_RUN_START, URHO3D_HANDLER(AnimationController, OnRunStart));
    SubscribeToEvent(node_->GetParent(), EVENT_RUN_STOP, URHO3D_HANDLER(AnimationController, OnRunStop));
    SubscribeToEvent(node_->GetParent(), EVENT_JUMP_START, URHO3D_HANDLER(AnimationController, OnJumpStart));
    SubscribeToEvent(node_->GetParent(), EVENT_JUMP_STOP, URHO3D_HANDLER(AnimationController, OnJumpStop));
    SubscribeToEvent(node_->GetParent(), EVENT_SHOOT, URHO3D_HANDLER(AnimationController, OnShoot));
}

void AnimationController::OnRunStart(StringHash eventType, VariantMap &eventData) {
    pushAnimation(Animation(AnimationPriority::Run, true, "run"));
}

void AnimationController::OnJumpStart(StringHash eventType, VariantMap &eventData) {
    pushAnimation(Animation(AnimationPriority::Jump, false, "jump"));
}

void AnimationController::OnRunStop(StringHash eventType, VariantMap &eventData) {
    pullAnimation(AnimationPriority::Run);
}

void AnimationController::OnJumpStop(StringHash eventType, VariantMap &eventData) {
    pullAnimation(AnimationPriority::Jump);
}

void AnimationController::OnShoot(StringHash eventType, VariantMap &eventData) {
    const auto target = eventData[EventDoShoot::P_TARGET].GetVector2();
    pushAnimation(Animation(AnimationPriority::Shoot, false, "shoot", 0.5f));
}

void AnimationController::PostUpdate(float timeStep) {
    const auto direction = node_->GetParent()->GetVar(PROP_DIRECTION).GetVector2();
    auto sprite = node_->GetComponent<AnimatedSprite2D>();
    sprite->SetFlipX(direction == Vector2::LEFT);

    if (timeout > 0.0f) {
        timeout -= timeStep;

        if (timeout <= 0.0f) {
            pullAnimation(current);
        }
    }
}

void AnimationController::pullAnimation(const AnimationController::AnimationPriority &priority) {
    for (auto it = animations.rbegin(); it != animations.rend(); it++) {
        if (it->GetPriority() == priority) {
            animations.erase(std::next(it).base());
            break;
        }
    }

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
        URHO3D_LOGERROR("Unable to determine animation");
    }

    auto it = animations.rbegin();
    if (it != animations.rend() && it->GetPriority() != current) {
        current = it->GetPriority();
        sprite->SetAnimation(it->GetName(), it->IsLoop() ? LoopMode2D::LM_FORCE_LOOPED : LM_FORCE_CLAMPED);
        if (it->GetTimeout() > 0.0f) {
            timeout = it->GetTimeout();
        }
    }
}
