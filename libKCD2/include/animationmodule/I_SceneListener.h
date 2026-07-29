#pragma once

// -----------------------------------------------
// wh::animationmodule::I_SceneListener -- animation/physics collision listener
// (KCD2 WHGame.dll 1.5.6). sizeof 0x08 (vtable only).
// -----------------------------------------------
// RTTI .?AVI_SceneListener@animationmodule@wh@@. Modelled minimally, from the C_CombatScene
// subobject that implements it (base at +0x08; subobject vtable 0x183A6C7A0 = 2 slots: [0] dtor
// thunk sub_1821392BC, [1] the listener callback -- a nop sub_1803B6E80 in C_CombatScene).
// C_CombatActorCollisions' implementation (0x180D51854) proves three arguments: two nullable
// collision-part descriptors and an EventPhysCollision. The concrete types of the first two
// descriptors and the canonical method name remain unverified.

struct EventPhysCollision;

namespace wh::animationmodule {

class I_SceneListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SceneListener;
    virtual ~I_SceneListener() = default;     // [0]
    virtual void OnCollision(const void* participant0,
                             const void* participant1,
                             const EventPhysCollision& collision) = 0;  // [1] name tentative
};

}  // namespace wh::animationmodule
