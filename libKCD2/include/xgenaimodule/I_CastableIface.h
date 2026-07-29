#pragma once
#include "I_NoRTTITypeInfo.h"

// -----------------------------------------------
// wh::xgenaimodule::I_CastableIface : I_NoRTTITypeInfo -- cast/type-query
// interface layer of the SUBB fleet base chain (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08 (vptr only, no data).
// -----------------------------------------------
// RTTI mangled .?AVI_CastableIface@xgenaimodule@wh@@.  Never most-derived.
// Contributes the cast-query slots [2..4] of C_SUBBBase's 37-slot table
// (slot [2] default return 0, overridden by Smart* cast thunks sub_1811D12xx);
// exact [0..4] split vs I_NoRTTITypeInfo is UNVERIFIED [U].

namespace wh::xgenaimodule {

class I_CastableIface : public I_NoRTTITypeInfo {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CastableIface;
    virtual void* CastQuery2();   // [2] default return 0 (sub_18066CD10); Smart* cast thunk [U]
    virtual void  _vf3();         // [3] [U role/split]
    virtual void  _vf4();         // [4] [U role/split]
};
static_assert(sizeof(I_CastableIface) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
