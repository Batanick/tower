#pragma once
//
// Created by botanick on 17.01.16.
//

#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class Named : public LogicComponent {

URHO3D_OBJECT(Named, LogicComponent)

public:
    Named(Context *context) : LogicComponent(context) {
        SetUpdateEventMask(USE_FIXEDUPDATE);
    }

    virtual void FixedUpdate(float timeStep) override;
    virtual void DelayedStart() override;

private:
    SharedPtr<Node> textNode;

};

