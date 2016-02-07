#pragma once


#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class Legat : public LogicComponent {

URHO3D_OBJECT(Legat, LogicComponent)

public:
    void static RegisterObject(Context *context);
    virtual void FixedUpdate(float timeStep) override;

    Legat(Context *context) : LogicComponent(context) {
        // nothing
    }


    void DelayedStart();
    void OnMouseDown(StringHash eventType, VariantMap &eventData);
};




