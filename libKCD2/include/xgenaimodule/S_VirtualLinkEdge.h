#pragma once
#include "S_LinkEdge.h"

// -----------------------------------------------
// wh::xgenaimodule::S_VirtualLinkEdge : S_LinkEdge -- "virtual" (indirect/computed)
// edge variant (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof NOT create-site proven ->
// no size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4F5D6D8 (.?AUS_VirtualLinkEdge@...).  vtable 0x183AE4920, 2 slots
// (same count as the base -- overrides only, no new slots):
// [0] deleting dtor sub_1832F56EC, [1] sub_18138B228 (base slot is nullsub).

namespace wh::xgenaimodule {

class S_VirtualLinkEdge : public S_LinkEdge {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_VirtualLinkEdge;
    ~S_VirtualLinkEdge() override;   // [0] deleting dtor sub_1832F56EC
    void _vf1() override;            // [1] sub_18138B228 [U role]
};

}  // namespace wh::xgenaimodule
