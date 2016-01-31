#pragma once


#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class Bullet : public LogicComponent {

URHO3D_OBJECT(Bullet, LogicComponent)

public:
    void static RegisterObject(Context *context);

    Bullet(Context *context) :
            LogicComponent(context),
            damage(1) {
        // nothing
    }

private:
    int damage;

    void OnHit(StringHash eventType, VariantMap &eventData);
public:
    virtual void DelayedStart() override;
    void Hit(Node *const victim) const;
};




