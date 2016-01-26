#pragma once

//
// Created by botanick on 17.01.16.
//

#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Math/Vector2.h>

namespace Urho3D {
    class Scene;
    class Node;
    class Context;
}

using namespace Urho3D;

class GameFactory {
public:
    void static InitContext(Context *context);
    void InitScene(const SharedPtr<Scene> scene);


    void Box(const Vector2 &position);
    void Wall(const Vector2 &position, const Vector2 &scale);

    void MainPlayer(const Vector2 &position);

private:
    SharedPtr<Scene> scene;
};

