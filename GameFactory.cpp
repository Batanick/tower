//
// Created by botanick on 17.01.16.
//

#include "GameFactory.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/SpriteSheet2D.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Graphics/AnimatedModel.h>

#include "Named.h"
#include "ManualMoveController.h"

#include "Properties.h"
#include "FollowCamera.h"
#include "AnimationController.h"
#include "FireController.h"
#include "Bullet.h"
#include "Mortal.h"

void GameFactory::InitContext(Context *context) {
    Named::RegisterObject(context);
    ManualMoveController::RegisterObject(context);
    FollowCamera::RegisterObject(context);
    AnimationController::RegisterObject(context);
    FireController::RegisterObject(context);
    Bullet::RegisterObject(context);
    Mortal::RegisterObject(context);
}

void GameFactory::InitScene(const SharedPtr<Scene> scene) {
    this->scene = scene;
}

void GameFactory::Box(const Vector2 &position) {
    ResourceCache *cache = scene->GetSubsystem<ResourceCache>();
    Sprite2D *boxSprite = cache->GetResource<Sprite2D>("Urho2D/Box.png");

    Node *node = scene->CreateChild("RigidBody");

    node->SetPosition(position);

    // Create rigid body
    RigidBody2D *body = node->CreateComponent<RigidBody2D>();
    body->SetBodyType(BT_DYNAMIC);
    body->SetMass(2.5f);
    body->SetLinearDamping(2.0f);

    StaticSprite2D *staticSprite = node->CreateComponent<StaticSprite2D>();
    staticSprite->SetSprite(boxSprite);

    // Create box
    CollisionBox2D *box = node->CreateComponent<CollisionBox2D>();
    // Set size
    box->SetSize(Vector2(0.32f, 0.32f));
    // Set friction
    box->SetFriction(0.0f);

    node->CreateComponent<Mortal>();
}

void GameFactory::Wall(const Vector2 &position, const Vector2 &scale) {
    ResourceCache *cache = scene->GetSubsystem<ResourceCache>();
    Sprite2D *boxSprite = cache->GetResource<Sprite2D>("Urho2D/Box.png");

    // Create ground.
    Node *groundNode = scene->CreateChild("Ground");
    groundNode->SetPosition(position);
    groundNode->SetScale(Vector3(scale.x_ / 0.32f, scale.y_ / 0.32f, 0.0f));

    // Create 2D rigid body for gound
    RigidBody2D *groundBody = groundNode->CreateComponent<RigidBody2D>();

    StaticSprite2D *groundSprite = groundNode->CreateComponent<StaticSprite2D>();
    groundSprite->SetSprite(boxSprite);

    // Create box collider for ground
    CollisionBox2D *groundShape = groundNode->CreateComponent<CollisionBox2D>();
    // Set box size
    groundShape->SetSize(Vector2(0.32f, 0.32f));
    // Set friction
    groundShape->SetFriction(0.0f);
}

void GameFactory::MainPlayer(const Vector2 &position) {
    ResourceCache *cache = scene->GetSubsystem<ResourceCache>();
    const auto animationSet = cache->GetResource<AnimationSet2D>("hero/hero.scml");

    Node *node = scene->CreateChild("");
    node->SetPosition(position);
    node->SetScale2D(Vector2(1.0f, 1.0f));

    node->SetVar(PROP_NAME, "Botanick");
    node->CreateComponent<Named>();
    node->CreateComponent<FollowCamera>();

    const auto pRigidBody2D = node->CreateComponent<RigidBody2D>();
    pRigidBody2D->SetBodyType(BT_DYNAMIC);
    pRigidBody2D->SetFixedRotation(true);
    pRigidBody2D->SetMass(10.0);
    pRigidBody2D->SetLinearDamping(2.0f);

    node->CreateComponent<ManualMoveController>();
    node->SetVar(PROP_SPEED, 4.0f);
    node->SetVar(PROP_JUMP_SPEED, 8.0f);

    auto sprite = node->CreateComponent<AnimatedSprite2D>();
    sprite->SetAnimationSet(animationSet);
    sprite->SetAnimation("idle");
    node->CreateComponent<AnimationController>();

    // Create box
    CollisionBox2D *box = node->CreateComponent<CollisionBox2D>();
    // Set size
    box->SetSize(Vector2(0.93f, 1.1f));
    box->SetGroupIndex(-((short) node->GetID())); // setting up collision ignore group

    node->CreateComponent<FireController>();
}
