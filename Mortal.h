#pragma once


#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class Mortal : public LogicComponent {

URHO3D_OBJECT(Mortal, LogicComponent)

public:
    void static RegisterObject(Context *context);

    Mortal(Context *context) :
            LogicComponent(context),
            alive(true),
            health(1) {
        //nothing
    }

private:
    int health;
    bool alive;

    void ReceiveDamage(StringHash eventType, VariantMap &eventData);
    void Die();
public:
    virtual void Start() override;

    int getHealth() const {
        return health;
    }

    bool isAlive() const {
        return alive;
    }

    virtual void FixedPostUpdate(float timeStep) override;
};