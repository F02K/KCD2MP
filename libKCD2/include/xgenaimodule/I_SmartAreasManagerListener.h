#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SmartAreasManagerListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (sizeof 0x08).
// -----------------------------------------------
// RTTI .?AVI_SmartAreasManagerListener@xgenaimodule@wh@@.  Callback interface of the smart-area
// manager. Slot semantics read from the single enumerated implementation, C_RPGLocationManager
// (subobject @+0x10, vtable 0x183AB63E0, 3 slots):
//   [0] 0x1803B6E80  shared ret stub (no-op in this impl)
//   [1] 0x180D9B214  area removed/unloaded: resolves the area's 8-byte key (arg+24 object ->
//                    vfn[3] -> vfn[2]) and calls I_RPGLocationManager::UnregisterLocationKey
//   [2] 0x1803B6E80  shared ret stub
// Slot count is the implementor's vtable extent (3); names INFERRED from that one impl.

namespace wh::xgenaimodule {

class I_SmartAreasManagerListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartAreasManagerListener;
    virtual void _vf0() = 0;                          // [0] 0x00
    virtual void OnSmartAreaRemoved(void* area) = 0;  // [1] 0x08  [name INFERRED]
    virtual void _vf2() = 0;                          // [2] 0x10
};
static_assert(sizeof(I_SmartAreasManagerListener) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::xgenaimodule
