#pragma once
//
// Created by botanick on 17.01.16.
//

#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class Named : public LogicComponent {

public:
    Named(Context *context) : LogicComponent(context) {
    }

private:
    virtual void Update(float timeStep) override;


public:
    virtual void Start() override;
public:

private:
    SharedPtr<Node> textNode;
protected:
public:
    virtual void FixedUpdate(float timeStep) override;
    virtual void DelayedStart() override;
};

