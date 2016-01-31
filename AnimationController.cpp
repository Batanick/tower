//
// Created by botanick on 31.01.16.
//

#include <Urho3D/Core/Context.h>

#include "AnimationController.h"

void AnimationController::RegisterObject(Context *context) {
    context->RegisterFactory<AnimationController>();
}

void AnimationController::FixedUpdate(float timeStep) {

}



