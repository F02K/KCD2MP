#pragma once
#include "I_AIPuppet.h"

// -----------------------------------------------
// wh::xgenaimodule::C_CachedPuppet : I_AIPuppet -- partial-impl puppet base (KCD2
// WHGame.dll 1.5.6, kd7u).  Modeled vptr-only (0x08); still abstract.
// -----------------------------------------------
// RTTI TD rva 0x4FF5E28; vtable 0x183A398D8, 25 slots: implements only [0] (deleting
// dtor sub_183446360 -> dtor sub_18047B0E0) and [24] (sub_180F8CC10, overriding the
// nullsub default); [15]/[21..23] keep the I_AIPuppet defaults; everything else stays
// _purecall. NO standalone create site exists -- its ctor is always inlined into the
// two C_AIPuppet ctors, so the member split between C_CachedPuppet and C_AIPuppet is
// UNOBSERVABLE; ALL data members (the cached-transform block etc.) are declared on
// C_AIPuppet in this mirror. True subobject size UNVERIFIED (<= 0x48).

namespace wh::xgenaimodule {

class C_CachedPuppet : public I_AIPuppet {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CachedPuppet;
    ~C_CachedPuppet() override;   // [0]  deleting dtor sub_183446360 -> dtor sub_18047B0E0
    void _vf24() override;        // [24] sub_180F8CC10 (vs nullsub default) [U role]
};
static_assert(sizeof(C_CachedPuppet) == 0x08, "C_CachedPuppet modeled vptr-only (member split vs C_AIPuppet unobservable)");

}  // namespace wh::xgenaimodule
