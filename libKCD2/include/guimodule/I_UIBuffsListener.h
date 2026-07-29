#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::I_UIBuffsListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 4 slots.
// -----------------------------------------------
// RTTI .?AVI_UIBuffsListener@guimodule@wh@@ (TD 0x184C925E0). Sink notified by
// C_UIBuffsManager (embedded C_Listeners<I_UIBuffsListener,1> @manager+0x10) about
// HUD-visible player-buff changes. Listeners register by pushing the subobject pointer
// into the manager's listener vector (e.g. C_UIHudBuffs::Init sub_180C3CB20 pushes
// this+0x58 into *(module+0x38)+0x18).
//
// Shape: exactly 4 slots, NO virtual dtor, NO rttr trio -- certified twice:
// C_UIHudBuffs @+0x58 (vtable 0x183C34678: 0x180C4BAB8/0x18051472C/0x181276840/
// 0x181954374 = flash "AddBuff"-family/"RemoveBuff"/"UpdateBuff"/"SwapBuffId") and
// C_UIHudAdaptive @+0x10 (vtable 0x183EEB838: [0] 0x181F782F0 sets the buffs-dirty
// flag, [1..3] nullsub defaults).
// Names coined from the flash call names; buff identity flows as a WUID-like id that
// the impls stringify via sub_1807EA6B0.

namespace wh::guimodule {

class I_UIBuffsListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIBuffsListener;
    virtual void OnBuffAdded(void* a2, void* pBuff) {}                       // [0] (C_UIHudBuffs 0x180C4BAB8: filters pBuff UI type sub_181F98670 in {1,3}; a2 = lookup key sub_180C4BC60)
    virtual void OnBuffRemoved(uint64_t buffId) {}                           // [1] (0x18051472C flash "RemoveBuff")
    virtual void OnBuffUpdated(uint64_t buffId, float progress, int a4) {}   // [2] (0x181276840 flash "UpdateBuff")
    virtual void OnBuffIdSwapped(uint64_t newId, uint64_t oldId) {}          // [3] (0x181954374 flash "SwapBuffId"; arg order UNVERIFIED)
};
static_assert(sizeof(I_UIBuffsListener) == 8);

}  // namespace wh::guimodule
