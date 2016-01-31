#pragma once


#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class FireController : public LogicComponent {

URHO3D_OBJECT(FireController, LogicComponent
)

public:
    void static RegisterObject(Context *context);

    FireController(Context *context) : LogicComponent(context) {
        // nothing
    }

    virtual void DelayedStart() override;
private:
    void OnMouseDown(StringHash eventType, VariantMap &eventData);
    void Shoot(Vector2 target);
    void CreateBullet(Vector2 direction);
};




