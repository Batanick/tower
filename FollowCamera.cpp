//
// Created by botanick on 25.01.16.
//

#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/OpenGL/OGLGraphics.h>

#include "FollowCamera.h"
#include "Properties.h"
#include "MathUtils.h"

const static float maxDx = 1.0f;
const static float maxDy = 1.0f;

void FollowCamera::Update(float timeStep) {
    const auto camera = GetScene()->GetChild(NAME_CAMERA);
    if (!camera)
        return;

    const auto characterPos = GetNode()->GetPosition();
    const auto currentPos = camera->GetPosition2D();
    const Vector2 cameraPos{trim(characterPos.x_ - maxDx, characterPos.x_ + maxDx, currentPos.x_),
                            trim(characterPos.y_ - maxDy, characterPos.y_ + maxDy, currentPos.y_)};
    camera->SetPosition(cameraPos);
}

FollowCamera::FollowCamera(Context *context) : LogicComponent(context) {
    // nothing here
}

void FollowCamera::RegisterObject(Context *context) {
    context->RegisterFactory<FollowCamera>();
}
