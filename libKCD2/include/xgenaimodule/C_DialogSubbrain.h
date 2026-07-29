#pragma once
#include "C_BehaviorTreeSUBB.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DialogSubbrain : C_BehaviorTreeSUBB -- dialog subbrain
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x120 (create-site proven: factory
// sub_18041A4DC operator new(288) -> ctor sub_180413C50(p,1) -- bool arg 1 sets
// m_btFlags bit1 -> vptr swap).  NO new members.
// -----------------------------------------------
// RTTI TD rva 0x4EFDEB8; COL rva 0x451DC08; vtable 0x183FD7840, 40 slots
// (followed in .rdata by a string/enum table 0,"single",1,"indexed",2 -- DATA,
// not slots).  Differs from C_BehaviorTreeSUBB at: [1] dtor sub_18320863C,
// [27] type sub_181A72CF0, [38] sub_18066CD10 (return 0).

namespace wh::xgenaimodule {

class C_DialogSubbrain : public C_BehaviorTreeSUBB {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogSubbrain;
    void* GetSubbrainType() override;   // [27] sub_181A72CF0
    bool  _vf38() override;             // [38] sub_18066CD10 (return 0)
};
static_assert(sizeof(C_DialogSubbrain) == 0x120, "C_DialogSubbrain must be 0x120 (operator new(288))");

}  // namespace wh::xgenaimodule
