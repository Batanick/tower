#pragma once

//
// Created by botanick on 25.01.16.
//

#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class FollowCamera : public LogicComponent {

URHO3D_OBJECT(FollowCamera, LogicComponent)

public:
    void static RegisterObject(Context *context);

    FollowCamera(Context *context);

    virtual void Update(float timeStep) override;
};



