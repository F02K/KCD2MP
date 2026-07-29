#pragma once
#include <cstdint>
#include "E_EventState.h"

// -----------------------------------------------
// wh::rpgmodule::I_Event -- refcounted event interface (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D40F00  vtable 0x183F1A6B0 (5 slots). Concrete impl is C_Event (see C_Event.h);
// held via _smart_ptr<I_Event> at S_DamageEventData+0x000 in all five verified producers.

namespace wh::rpgmodule {

class C_EffectVisitor;

class I_Event {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Event;
    virtual ~I_Event() = default;                            // [0] 0x00
    virtual void VisitEffects(C_EffectVisitor& visitor) = 0; // [1] 0x08
    virtual E_EventState GetState() const = 0;               // [2] 0x10
    virtual void AddRef() = 0;                                // [3] 0x18
    virtual void Release() = 0;                               // [4] 0x20
};
static_assert(sizeof(I_Event) == 0x08, "I_Event is a vfptr-only interface");

}  // namespace wh::rpgmodule
