#pragma once

//
// Created by botanick on 17.01.16.
//

#include <Urho3D/Container/Ptr.h>

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


    void Box();
    void Ball();
    void Wall();

    void MainPlayer();

private:
    SharedPtr<Scene> scene;
};

