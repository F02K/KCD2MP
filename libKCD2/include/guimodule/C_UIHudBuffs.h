#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashBase.h"
#include "guimodule/I_UIBuffsListener.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudBuffs -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60.
// -----------------------------------------------
// RTTI .?AVC_UIHudBuffs@guimodule@wh@@ (TD 0x184CCFD30).
// Bases: C_UIFlashBase @+0x00 (COL 0x4135D60), IUIElementEventListener @+0x10
// (COL 0x4135D88), I_UIBuffsListener @+0x58 (COL 0x4135DB0).
// vtables: primary 0x183C346E8 (14; GetElementName [11] 0x181A8E8F0 = "hud"),
// listener 0x183C346A0 (8), buffs-listener 0x183C34678 (4: 0x180C4BAB8 /
// 0x18051472C / 0x181276840 / 0x181954374). ctor inlined in the rttr
// as_std_shared_ptr creator sub_18188E584 (alloc 0x60). NO own data members.
//
// The buff-icon strip on the "hud" movie -- pure presentation leaf of the
// C_UIBuffsManager feed: Init [1] sub_180C3CB20 pushes the I_UIBuffsListener
// subobject (this+0x58) into the manager's listener vector (*(module+0x38)+0x18);
// the four listener callbacks forward to flash ("RemoveBuff"/"UpdateBuff"/
// "SwapBuffId" + the add path).
//
// [FUNDAMENTAL vs KCD1] Replaces the buff part of KCD1's C_UIRPGElements event system.

namespace wh::guimodule {

class C_UIHudBuffs : public C_UIFlashBase, public I_UIBuffsListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudBuffs;
    // I_UIBuffsListener overrides (vtable 0x183C34678).
    void OnBuffAdded(void* a2, void* pBuff) override;                        // [0] 0x180C4BAB8
    void OnBuffRemoved(uint64_t buffId) override;                            // [1] 0x18051472C  flash "RemoveBuff"
    void OnBuffUpdated(uint64_t buffId, float progress, int a4) override;    // [2] 0x181276840  flash "UpdateBuff"
    void OnBuffIdSwapped(uint64_t newId, uint64_t oldId) override;           // [3] 0x181954374  flash "SwapBuffId"
};
static_assert(sizeof(C_UIHudBuffs) == 0x60, "C_UIHudBuffs must be 0x60");

}  // namespace wh::guimodule
