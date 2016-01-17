#pragma once

//
// Created by botanick on 17.01.16.
//

#include <Urho3D/Container/Ptr.h>

namespace Urho3D {
    class Scene;
    class Node;
}

using namespace Urho3D;

class GameFactory {
public:
    void Init(const SharedPtr<Scene> scene);

    void Box();
    void Ball();
    void Wall();

private:
    SharedPtr<Scene> scene;
};

