//
// Created by botanick on 17.01.16.
//

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/OpenGL/OGLGraphics.h>
#include <Urho3D/Core/Context.h>

#include "GameScene.h"
#include "Properties.h"

static const unsigned NUM_OBJECTS = 100;

void GameScene::Init(Context *context) {
    GameFactory::InitContext(context);

    scene = new Scene(context);
    scene->CreateComponent<Octree>();
    scene->CreateComponent<DebugRenderer>();

    const auto pPhysicsWorld2D = scene->CreateComponent<PhysicsWorld2D>();
    pPhysicsWorld2D->SetAutoClearForces(true);

    // Create camera node
    cameraNode = scene->CreateChild(NAME_CAMERA);
    // Set camera's position
    cameraNode->SetPosition(Vector3(0.0f, 0.0f, -10.0f));

    Camera *camera = cameraNode->CreateComponent<Camera>();
    camera->SetOrthographic(true);

    Graphics *graphics = context->GetSubsystem<Graphics>();
    camera->SetOrthoSize((float) graphics->GetHeight() * PIXEL_SIZE);
    // Set zoom according to user's resolution to ensure full visibility (initial zoom (1.2) is set for full visibility at 1280x800 resolution)
    camera->SetZoom(1.2f * Min((float) graphics->GetWidth() / 1280.0f, (float) graphics->GetHeight() / 800.0f));

    factory = GameFactory();
    factory.InitScene(scene);

    factory.Wall();

    for (int i = 0; i < 5; i++) {
        factory.Box();
        factory.Box();
    }

    factory.MainPlayer();
}