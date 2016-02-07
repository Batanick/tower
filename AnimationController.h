#pragma once

#include <Urho3D/Scene/LogicComponent.h>
#include <set>

using namespace Urho3D;

class AnimationController : public LogicComponent {

URHO3D_OBJECT(AnimationController, LogicComponent)

public:
    void static RegisterObject(Context *context);

    AnimationController(Context *context);

    virtual void Start() override;

private:
    void OnRunStart(StringHash eventType, VariantMap &eventData);
    void OnRunStop(StringHash eventType, VariantMap &eventData);
    void OnJumpStart(StringHash eventType, VariantMap &eventData);
    void OnJumpStop(StringHash eventType, VariantMap &eventData);
    void OnShoot(StringHash eventType, VariantMap &eventData);

    enum AnimationPriority {
        Idle, Run, Jump, Shoot
    };

    class Animation {
    public:
        Animation(const AnimationPriority &priority, const bool loop, const String &name)
                : priority(priority), loop(loop), name(name) { }

        const AnimationPriority &GetPriority() const {
            return priority;
        }

        bool IsLoop() const {
            return loop;
        }


        const String &GetName() const {
            return name;
        }

        const bool operator<(const Animation &r) const {
            return priority < r.priority;
        }

    private:
        const AnimationPriority priority;
        const bool loop;
        const String name;

    };

    std::set<Animation> animations;
    AnimationPriority current;

    void refresh();
    void pushAnimation(const Animation &animation);
    void pullAnimation(const AnimationPriority &animation);
public:
    virtual void FixedPostUpdate(float timeStep) override;
};




