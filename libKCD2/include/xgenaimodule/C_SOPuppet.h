#pragma once
#include "C_AIPuppet.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SOPuppet : C_AIPuppet -- smart-object puppet (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x48 (create-site proven: tracked alloc 72).
// -----------------------------------------------
// RTTI TD rva 0x4FFB9A8; vtable 0x183A6B9E8, 25 slots -- byte-identical to the
// C_AIPuppet vtable except [0] (own deleting dtor). NO extra members. Built by creator
// sub_180A2A308(mgr, host, wuid*, hostSO, name): base ctor sub_180A2A458 (4-arg, with
// staticFlag = !entity("bMovingSmartObject")) then leaf vtable overwrite.

namespace wh::xgenaimodule {

class C_SOPuppet : public C_AIPuppet {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SOPuppet;
    ~C_SOPuppet() override;   // [0] own deleting dtor; slots [1..24] inherited unchanged
};
static_assert(sizeof(C_SOPuppet) == 0x48, "C_SOPuppet adds no members to C_AIPuppet");

}  // namespace wh::xgenaimodule
