#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::I_SourceMonitorListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 1 slot.
// -----------------------------------------------
// RTTI .?AVI_SourceMonitorListener@framework@wh@@. Callback interface of the framework
// "source monitor"; listeners register by name+type on the monitor object at
// *(frameworkGetter sub_1823CBF70()+0xD8) via vf[+0x28](CryStringT name, int type, listener)
// (observed: C_UIFullUIModeHelper ctor @0x18194af2c, type 6).
//
// Shape: ONE slot, NO virtual dtor -- certified twice: C_MissileWeaponPlayerController's
// subobject vtable 0x183A80E20 (1 slot, callback sub_1814DBC2C) and C_UIFullUIModeHelper's
// primary vtable 0x183EDE9B0 ([0] callback sub_181F52710; its [1] deleting dtor is the
// HELPER's own virtual dtor, not an interface slot).
// Param roles from the C_UIFullUIModeHelper override: a2 = out-slot the impl
// default-initializes, bActive = enter/leave flag. Method name + exact param types
// UNVERIFIED (coined); originally declared inline in C_MissileWeaponPlayerController.h,
// moved here (one class per header).

namespace wh::framework {

class I_SourceMonitorListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SourceMonitorListener;
    virtual void OnSourceEvent(void* a2, bool bActive) = 0;   // [0]
};
static_assert(sizeof(I_SourceMonitorListener) == 8);

}  // namespace wh::framework
