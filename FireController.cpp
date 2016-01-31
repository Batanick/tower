//
// Created by botanick on 31.01.16.
//

#include "FireController.h"
#include "Properties.h"
#include "Bullet.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/CollisionCircle2D.h>

void FireController::RegisterObject(Context *context) {
    context->RegisterFactory<FireController>();
}

void FireController::DelayedStart() {
    SubscribeToEvent(E_UIMOUSECLICK, URHO3D_HANDLER(FireController, OnMouseDown));
}


void FireController::OnMouseDown(StringHash eventType, VariantMap &eventData) {
    const auto cameraNode = GetScene()->GetChild(NAME_CAMERA);
    if (!cameraNode) {
        return;
    }

    const auto pCamera = cameraNode->GetComponent<Camera>();
    if (!pCamera) {
        return;
    }

    const auto graphics = GetSubsystem<Graphics>();

    float x = (float) eventData[UIMouseClick::P_X].GetInt();
    float y = (float) eventData[UIMouseClick::P_Y].GetInt();

    auto target = pCamera->ScreenToWorldPoint(Vector3(x / graphics->GetWidth(), y / graphics->GetHeight(), 0.0f));

    Shoot(Vector2(target.x_, target.y_));
}

void FireController::Shoot(Vector2 target) {
    Vector2 direction = target - node_->GetPosition2D();
    direction.Normalize();

    CreateBullet(direction);
}

void FireController::CreateBullet(Vector2 direction) {
    ResourceCache *cache = GetScene()->GetSubsystem<ResourceCache>();
    Sprite2D *boxSprite = cache->GetResource<Sprite2D>("Urho2D/Ball.png");

    Node *bullet = GetScene()->CreateChild("RigidBody");
    bullet->SetPosition(node_->GetPosition());

    // Create rigid body
    RigidBody2D *body = bullet->CreateComponent<RigidBody2D>();
    body->SetBodyType(BT_DYNAMIC);
    body->ApplyLinearImpulse(direction * 100, body->GetMassCenter(), true);

    StaticSprite2D *staticSprite = bullet->CreateComponent<StaticSprite2D>();
    staticSprite->SetSprite(boxSprite);

    CollisionCircle2D *collision = bullet->CreateComponent<CollisionCircle2D>();
    collision->SetRadius(0.17);
    collision->SetTemporary(true);
    collision->SetGroupIndex(-((short) node_->GetID())); // did not collide with owner

    bullet->CreateComponent<Bullet>();

    bullet->Scale(0.3f);
}
