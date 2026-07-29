#pragma once
#include <cstdint>
#include <vector>
#include "I_Event.h"
#include "I_Cause.h"

// -----------------------------------------------
// wh::rpgmodule::C_Event -- concrete refcounted event (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D40F30  vtable 0x183A68568. sizeof 0x38 (PROVEN: ctor sub_1809DB5A4 tracked-alloc
// 56, dword_1851DFB18 += 0x38). Base is I_Event (see I_Event.h), not a standalone vtable --
// corrects an earlier pass that modeled this class without its base interface and treated
// +0x10..+0x2F as an opaque "reputation slots" blob.
//
// Ctor moves the caller's cause pointer into +0x08 (source nulled -> ownership transfer),
// zero-inits the effects vector/state/refcount, then tail-calls vtbl[3] (AddRef). Owns an
// I_Cause and a vector of C_Effect* that self-register via C_Effect's own ctor (push into
// +0x10, growth-checked sub_18040F4E4 / in-place sub_18240E2D0). AddRef/Release
// (sub_1809DB684/sub_1809DB694) interlocked-inc/dec +0x2C; Release deletes this (vtbl[0]) at
// zero. Dtor sub_1809DB6C0 -> cleanup sub_1809DB704 deletes every effect, frees the vector,
// then deletes m_pCause.

namespace wh::rpgmodule {

class C_Effect;

class C_Event : public I_Event {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Event;
    ~C_Event() override;                                    // [0] 0x1809DB6C0
    void VisitEffects(C_EffectVisitor& visitor) override;   // [1] 0x1809DB430
    E_EventState GetState() const override;                 // [2] 0x180836120
    void AddRef() override;                                 // [3] 0x1809DB684
    void Release() override;                                // [4] 0x1809DB694

    I_Cause* m_pCause;                 // +0x08  owned; moved in by ctor, source nulled
    std::vector<C_Effect*> m_effects;  // +0x10  owned; elements self-register via C_Effect ctor
    E_EventState m_state;              // +0x28
    volatile int32_t m_refCount;       // +0x2C  interlocked
    bool m_hasFlaggedTargetSoul;       // +0x30  OR of C_Effect::IsTargetSoulFlagged() [role tentative]
    uint8_t _pad31[7];                 // +0x31
};
static_assert(sizeof(C_Event) == 0x38, "C_Event must be 0x38 (alloc size at ctor)");
static_assert(offsetof(C_Event, m_effects) == 0x10, "effects vector at 0x10");
static_assert(offsetof(C_Event, m_state) == 0x28, "state at 0x28");
static_assert(offsetof(C_Event, m_refCount) == 0x2C, "refcount at 0x2C");

}  // namespace wh::rpgmodule
