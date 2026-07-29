#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_NoRTTITypeInfo -- root type-query interface of the SUBB
// fleet base chain (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vptr only).
// -----------------------------------------------
// RTTI mangled .?AVI_NoRTTITypeInfo@xgenaimodule@wh@@.  Never most-derived; the
// C_SUBBBase dtor sub_180413AD8 restores I_NoRTTITypeInfo::vftable as the
// least-derived tail.  Exact slot ownership of C_SUBBBase slots [0..4] between
// this iface and I_CastableIface is UNVERIFIED [U] -- modeled minimally: [0] is
// the shared type-query slot (default return 0, sub_18066CD10; Smart* override
// with cast thunks), [1] the deleting dtor.

namespace wh::xgenaimodule {

class I_NoRTTITypeInfo {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_NoRTTITypeInfo;
    virtual void* TypeQuery0();     // [0] default return 0 (sub_18066CD10) [U role/split]
    virtual ~I_NoRTTITypeInfo();    // [1] scalar deleting dtor slot
};
static_assert(sizeof(I_NoRTTITypeInfo) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
