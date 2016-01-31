//
// Created by botanick on 31.01.16.
//

#include "Bullet.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Urho2D/PhysicsEvents2D.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/IO/Log.h>

#include "Properties.h"

void Bullet::RegisterObject(Context *context) {
    context->RegisterFactory<Bullet>();
}

void Bullet::DelayedStart() {
    SubscribeToEvent(E_PHYSICSBEGINCONTACT2D, URHO3D_HANDLER(Bullet, OnHit));
}

void Bullet::OnHit(StringHash eventType, VariantMap &eventData) {
    const auto obj1 = dynamic_cast<Node *>(eventData[PhysicsBeginContact2D::P_NODEA].GetPtr());
    const auto obj2 = dynamic_cast<Node *>(eventData[PhysicsBeginContact2D::P_NODEB].GetPtr());

    if (obj1 == node_) {
        Hit(obj2);
    } else if (obj2 == node_) {
        Hit(obj1);
    }

    node_->Remove();
}

void Bullet::Hit(Node *const victim) const {
    VariantMap params;
    params[ReceiveDamage::P_VALUE] = damage;
    victim->SendEvent(EVENT_RECEIVE_DAMAGE, params);
}
