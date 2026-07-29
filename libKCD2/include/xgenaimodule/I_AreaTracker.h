#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_AreaTracker -- area enter/leave tracking interface.
// KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (sizeof 0x08).
// -----------------------------------------------
// RTTI .?AVI_AreaTracker@xgenaimodule@wh@@ (TD 0x184C92788).  Survives from KCD1
// (KCD1 C_CompassMark also derived it, 4 slots).  Implementor analyzed here:
// guimodule::C_CompassMark (base @+0x00, vtable 0x183A6B4C8 = 4 slots, no dtor slot).
//
// All four C_CompassMark impls toggle the mark's m_playerInArea byte (+0x70):
//   [0] 0x181A8C560  flag = 1   (FLIRT mislabel "VirtualProcessorRoot::MarkRootRemoved")
//   [1] 0x181A8C570  flag = 0
//   [2] 0x182B162FC  if (flag) flag = 0
//   [3] 0x182B162FC  same impl as [2] (COMDAT-folded)
// Names INFERRED from that behavior; the tracker-manager caller side was not traced.

namespace wh::xgenaimodule {

class I_AreaTracker {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_AreaTracker;
    virtual void OnAreaEntered() = 0;   // [0]
    virtual void OnAreaLeft() = 0;      // [1]
    virtual void _vf2() = 0;            // [2] conditional leave/reset (role UNVERIFIED)
    virtual void _vf3() = 0;            // [3] conditional leave/reset (role UNVERIFIED)
};
static_assert(sizeof(I_AreaTracker) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::xgenaimodule
