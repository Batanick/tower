#pragma once

//
// Created by botanick on 17.01.16.
//

#include <Urho3D/Math/StringHash.h>

using namespace Urho3D;

// object properties
static const StringHash PROP_NAME{"name"};
static const StringHash PROP_SPEED{"speed"};
static const StringHash PROP_JUMP_SPEED{"jump_speed"};
static const StringHash PROP_IS_JUMPING{"jumping"};
static const StringHash PROP_IS_RUNNING{"running"};
static const StringHash PROP_DIRECTION{"direction"};

// unique object names
static const String NAME_CAMERA{"Camera"};
static const String NAME_MAIN_CHAR{"player"};
static const String NAME_ANIMATIONS{"animation"};

// =====================================================
// EVENTS
// =====================================================
//Animation
static const StringHash EVENT_RUN_START{"event_run_start"};
static const StringHash EVENT_RUN_STOP{"event_run_stop"};
static const StringHash EVENT_JUMP_START{"event_jump_start"};
static const StringHash EVENT_JUMP_STOP{"event_jump_stop"};
URHO3D_EVENT(EVENT_SHOOT, EventAnimationShoot) {
    URHO3D_PARAM(P_TARGET, Target);
}

// Mechanics
URHO3D_EVENT(EVENT_RECEIVE_DAMAGE, ReceiveDamage) {
    URHO3D_PARAM(P_VALUE, Damage);
}

// Controlls
URHO3D_EVENT(EVENT_DO_MOVE, EventDoMove) {
    URHO3D_PARAM(P_DIRECTION, Direction);
}

URHO3D_EVENT(EVENT_DO_JUMP, EventDoJump) {
}

URHO3D_EVENT(EVENT_DO_SHOOT, EventDoShoot) {
    URHO3D_PARAM(P_TARGET, Target);
}

