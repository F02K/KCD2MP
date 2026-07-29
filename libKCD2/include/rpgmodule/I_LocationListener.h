#pragma once

// -----------------------------------------------
// wh::rpgmodule::I_LocationListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (sizeof 0x08).
// -----------------------------------------------
// RTTI .?AVI_LocationListener@rpgmodule@wh@@.  Change-notify listener registered with
// C_RPGLocationManager (listener vector @manager+0x288, deferred-removal notify stack).
// Slots attested from MANAGER-side call sites only (no implementor vtable enumerated yet);
// the vtable has AT LEAST 10 slots -- trailing slots beyond [9] unknown, and the POI
// state-change notify walkers (sub_181FB76F0 / sub_181FB7680 / sub_181FB7520) dispatch
// through a packed event context whose slot indices are not resolved here.
// KCD1's replica (Offsets::I_LocationListener) had 9 slots.

namespace wh::rpgmodule {

class I_Location;
class I_RPGLocationManager;

class I_LocationListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_LocationListener;
    virtual void _vf0() = 0;   // [0] 0x00
    virtual void _vf1() = 0;   // [1] 0x08
    virtual void _vf2() = 0;   // [2] 0x10
    virtual void _vf3() = 0;   // [3] 0x18
    virtual void _vf4() = 0;   // [4] 0x20
    virtual void _vf5() = 0;   // [5] 0x28
    virtual void _vf6() = 0;   // [6] 0x30
    // [7] 0x38  called by I_RPGLocationManager::AddListener right after insertion [name INFERRED]
    virtual void OnRegistered(I_RPGLocationManager* mgr) = 0;
    // [8] 0x40  called by I_RPGLocationManager::RemoveListener before removal [name INFERRED]
    virtual void OnUnregistered(I_RPGLocationManager* mgr) = 0;
    // [9] 0x48  first actor enters a location (C_RPGLocation inside-counter 0 -> 1) [name INFERRED]
    virtual void OnLocationOccupied(I_RPGLocationManager* mgr, I_Location* loc) = 0;
};
static_assert(sizeof(I_LocationListener) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::rpgmodule
