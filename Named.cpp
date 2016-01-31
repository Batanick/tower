//
// Created by botanick on 17.01.16.
//

#include "Named.h"

#include <Urho3D/Scene/Node.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text3D.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Core/Context.h>

#include "Properties.h"

void Named::FixedUpdate(float) {
    if (textNode == nullptr)
        return;

    auto text = textNode->GetComponent<Text3D>();
    auto &variant = textNode->GetParent()->GetVar(PROP_NAME);
    text->SetText(variant.GetString());
}

void Named::DelayedStart() {
    auto node = GetNode();
    if (node == nullptr) {
        return;
    }

    node->GetVar("name");
    textNode = node->CreateChild("name");

    const auto sprite = node->GetDerivedComponent<Drawable2D>();
    const auto bBox = sprite->GetWorldBoundingBox();

    textNode->SetPosition(Vector3(0, 0.7, 0));

    auto text = textNode->CreateComponent<Text3D>();
    auto &variant = textNode->GetParent()->GetVar(PROP_NAME);
    text->SetText(variant.GetString());
    text->SetAlignment(HorizontalAlignment::HA_CENTER, VerticalAlignment::VA_TOP);
    text->SetColor(Color::GREEN);

    ResourceCache *cache = GetSubsystem<ResourceCache>();
    text->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
}

void Named::RegisterObject(Context *context) {
    context->RegisterFactory<Named>();
}
