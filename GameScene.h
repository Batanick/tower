#pragma once
//
// Created by botanick on 17.01.16.
//

#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>
#include "GameFactory.h"

using namespace Urho3D;

class GameScene : public RefCounted {

public:
    void Init(Context *context);

    const SharedPtr<Scene> &GetScene() const {
        return scene;
    }

    const SharedPtr<Node> &GetCamera() const {
        return cameraNode;
    }

private:
    SharedPtr<Scene> scene;
    SharedPtr<Node> cameraNode;
    GameFactory factory;
};
