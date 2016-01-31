#pragma once

//
// Created by botanick on 26.01.16.
//

#include "GameFactory.h"

using namespace Urho3D;

class SceneBuilder final {
public:
    void static build(GameFactory &factory) {
        factory.MainPlayer({0.0f, 0.2f});

        factory.Wall({0.0f, 0.0f}, {100.0f, 0.2f}); // floor
        factory.Wall({0.0f, 3.0f}, {10.0f, 0.2f}); //ceiling

        factory.Wall({-5.0f, 1.5f}, {0.2f, 3.0f});
        factory.Wall({5.0f, 1.5f}, {0.2f, 3.0f});

        for (int i = 0; i < 15; i++) {
            factory.Box({Random(-2.0f, -1.0f), Random(0.2f, 3.0f)});
        }
    }
};



