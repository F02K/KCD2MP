#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::I_Negativity -- interface of the "negativity" emotional-value
// family (KCD2 WHGame.dll 1.5.6, kd7u).  11 slots incl. slot-0 deleting dtor.
// -----------------------------------------------
// RTTI TD rva 0x4D4E188.  Impl: C_Nervousness (embedded @ C_NPCFactionNode+0x90);
// C_FactionAngriness is a likely sibling implementation [U].  Slot names derive
// from the C_Nervousness impl behavior (see C_Nervousness.h for EAs).

namespace wh::rpgmodule {

class I_Negativity {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Negativity;
    virtual ~I_Negativity();                             // [0] scalar deleting dtor slot
    virtual void  ResetThunk() = 0;                      // [1] thunk -> Reset [U name]
    virtual float UpdateAndGet() = 0;                    // [2] composite: ResolveOwner + Add [U name]
    virtual void  SetValue(float v) = 0;                 // [3] value = v; lastUpdate = 0
    virtual void  Reset(void* a2, bool propagate) = 0;   // [4] lastUpdate = now; value = Add(0); optional Propagate
    virtual float Add(float delta) = 0;                  // [5] clamp(value + delta, min, max)
    virtual float ClampAdd(float a, float b) = 0;        // [6] fminf(fmaxf(a+b, lo), hi)
    virtual float GetDecayRate() = 0;                    // [7] tuning-param getter
    virtual void* ResolveOwner() = 0;                    // [8] owner faction-node from game ctx
    virtual float Decay(uint64_t now) = 0;               // [9] rate * (now - lastUpdate) * 0.001
    virtual void  Propagate(void* a2) = 0;               // [10] spatial spread to nearby faction nodes
};
static_assert(sizeof(I_Negativity) == 0x08, "I_Negativity is vptr-only");

}  // namespace wh::rpgmodule
