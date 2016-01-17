//
// Created by botanick on 17.01.16.
//

#include "GameFactory.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionCircle2D.h>

#include "Named.h"
#include "Properties.h"

void GameFactory::Init(const SharedPtr<Scene> scene) {
    this->scene = scene;

    scene->GetContext()->RegisterFactory<Named>();
}

void GameFactory::Box() {
    ResourceCache *cache = scene->GetSubsystem<ResourceCache>();
    Sprite2D *boxSprite = cache->GetResource<Sprite2D>("Urho2D/Box.png");

    Node *node = scene->CreateChild("RigidBody");
    node->SetVar(NAME, "Companion");

    node->SetPosition(Vector3(Random(-0.1f, 0.1f), 5.0f * 0.4f, 0.0f));
    node->CreateComponent<Named>();

    // Create rigid body
    RigidBody2D *body = node->CreateComponent<RigidBody2D>();
    body->SetBodyType(BT_DYNAMIC);

    StaticSprite2D *staticSprite = node->CreateComponent<StaticSprite2D>();
    staticSprite->SetSprite(boxSprite);

    // Create box
    CollisionBox2D *box = node->CreateComponent<CollisionBox2D>();
    // Set size
    box->SetSize(Vector2(0.32f, 0.32f));
    // Set density
    box->SetDensity(1.0f);
    // Set friction
    box->SetFriction(0.5f);
    // Set restitution
    box->SetRestitution(0.1f);
}

void GameFactory::Ball() {
    ResourceCache *cache = scene->GetSubsystem<ResourceCache>();
    Sprite2D *ballSprite = cache->GetResource<Sprite2D>("Urho2D/Ball.png");

    Node *node = scene->CreateChild("RigidBody");
    node->SetPosition(Vector3(Random(-0.1f, 0.1f), 5.0f * 0.4f, 0.0f));

    // Create rigid body
    RigidBody2D *body = node->CreateComponent<RigidBody2D>();
    body->SetBodyType(BT_DYNAMIC);

    StaticSprite2D *staticSprite = node->CreateComponent<StaticSprite2D>();
    staticSprite->SetSprite(ballSprite);

    // Create circle
    CollisionCircle2D *circle = node->CreateComponent<CollisionCircle2D>();
    // Set radius
    circle->SetRadius(0.16f);
    // Set density
    circle->SetDensity(1.0f);
    // Set friction.
    circle->SetFriction(0.5f);
    // Set restitution
    circle->SetRestitution(0.1f);
}

void GameFactory::Wall() {
    ResourceCache *cache = scene->GetSubsystem<ResourceCache>();
    Sprite2D *boxSprite = cache->GetResource<Sprite2D>("Urho2D/Box.png");

    // Create ground.
    Node *groundNode = scene->CreateChild("Ground");
    groundNode->SetPosition(Vector3(0.0f, -3.0f, 0.0f));
    groundNode->SetScale(Vector3(200.0f, 1.0f, 0.0f));

    // Create 2D rigid body for gound
    /*RigidBody2D* groundBody = */groundNode->CreateComponent<RigidBody2D>();

    StaticSprite2D *groundSprite = groundNode->CreateComponent<StaticSprite2D>();
    groundSprite->SetSprite(boxSprite);

    // Create box collider for ground
    CollisionBox2D *groundShape = groundNode->CreateComponent<CollisionBox2D>();
    // Set box size
    groundShape->SetSize(Vector2(0.32f, 0.32f));
    // Set friction
    groundShape->SetFriction(0.5f);
}


