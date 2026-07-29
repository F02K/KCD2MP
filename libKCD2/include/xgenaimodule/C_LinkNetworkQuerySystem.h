#pragma once
#include <cstdint>
#include "I_LinkNetworkQuerySystem.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LinkNetworkQuerySystem : I_LinkNetworkQuerySystem -- the
// stateless link-graph query singleton (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08
// (create-site proven: alloc 8 in the lazy-init getter sub_1831A81A0 ->
// qword_1855AA178; vtable only, no data).
// -----------------------------------------------
// RTTI TD rva 0x4EC2A90; vtable 0x183FCC540, 3 slots.
// [0] GetLinks sub_183421E0C: reg = sub_18047C1F8(qword_185481FC8 /*C_AIObjectManager*/,
//     entity); obj = sub_1805D2764(reg) checked-cast; li = sub_18047AFA4(obj,obj,0)
//     edge iterate; sub_1820BBD20(li,&out) -> vector of 3-qword entries.
// [1] HasLink sub_183422CE0: resolve obj, iterate its link-name list, strcmp nameKey.
// [2] sub_1831A26E8 shared trivial.

namespace wh::xgenaimodule {

class C_LinkNetworkQuerySystem : public I_LinkNetworkQuerySystem {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LinkNetworkQuerySystem;
    void GetLinks(void* outVec, uint64_t entityId) override;      // [0] sub_183421E0C
    bool HasLink(uint64_t entityId, const char* nameKey) override; // [1] sub_183422CE0
    void _vf2() override;                                          // [2] sub_1831A26E8
};
static_assert(sizeof(C_LinkNetworkQuerySystem) == 0x08, "C_LinkNetworkQuerySystem is stateless (alloc 8)");

}  // namespace wh::xgenaimodule
