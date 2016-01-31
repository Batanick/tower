//
// Created by botanick on 31.01.16.
//

#include "Mortal.h"

#include "Properties.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>

void Mortal::RegisterObject(Context *context) {
    context->RegisterFactory<Mortal>();
}

void Mortal::Start() {
    SubscribeToEvent(node_, EVENT_RECEIVE_DAMAGE, URHO3D_HANDLER(Mortal, ReceiveDamage));
}

void Mortal::FixedPostUpdate(float timeStep) {
    if (!alive) {
        node_->Remove();
    }
}

void Mortal::ReceiveDamage(StringHash eventType, VariantMap &eventData) {
    if (!alive) {
        return;
    }

    health -= eventData[ReceiveDamage::P_VALUE].GetInt();
    if (health <= 0) {
        Die();
    }
}

void Mortal::Die() {
    health = 0;
    alive = false;
}
