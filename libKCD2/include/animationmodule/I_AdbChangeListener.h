#pragma once

// -----------------------------------------------
// wh::animationmodule::I_AdbChangeListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (0x08).
// -----------------------------------------------
// RTTI .?AUI_AdbChangeListener@animationmodule@wh@@ (TD, RVA 0x4B..; used as an MI base of the
// animation/combat object databases at mdisp 0x98). "Adb" = Animation DataBase. Databases that
// derive from the animation stack register as listeners so they get notified when the backing ADB
// (Mannequin animation database) is reloaded/changed and can rebuild their runtime tag resolutions.
//
// The concrete subobject vtable (as it appears inside C_CombatActionAttackDatabase @+0x98, VA
// 0x183E50CB0) has 2 slots:
//   [0] 0x18041A6A0  OnAdbChanged   -> returns 1 (accept/handled)   [name inferred]
//   [1] 0x1803B6E80  OnAdbReloaded  -> nullsub                       [name inferred]
// Both are behaviour-recovered defaults; the canonical method names are not in the binary.

namespace wh::animationmodule {

class I_AdbChangeListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_AdbChangeListener;
    virtual ~I_AdbChangeListener() = default;      // (no dtor slot of its own here; MI-thin)
    virtual bool OnAdbChanged() { return true; }   // [0] 0x18041A6A0  (name inferred)
    virtual void OnAdbReloaded() {}                // [1] 0x1803B6E80  (name inferred)
};

}  // namespace wh::animationmodule
