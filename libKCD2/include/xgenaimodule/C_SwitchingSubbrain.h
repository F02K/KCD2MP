#pragma once
#include "C_BehaviorTreeSUBB.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SwitchingSubbrain : C_BehaviorTreeSUBB -- the switching
// subbrain; NOTE: the brief's "C_SubbSwitch" does not exist as an RTTI type,
// THIS is the switching subbrain (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x120
// (create-site proven: factory sub_18041A4DC operator new(288) -> ctor
// sub_180413C50(p,0) -> vptr swap).  NO new members (layout == BehaviorTreeSUBB).
// -----------------------------------------------
// RTTI TD rva 0x4EFED48; COL rva 0x40E72A0; vtable 0x183A343B8, 40 slots --
// differs from C_BehaviorTreeSUBB at 3 slots only: [1] dtor sub_180D470A8,
// [27] type sub_181A72480, [38] sub_181A72470.

namespace wh::xgenaimodule {

class C_SwitchingSubbrain : public C_BehaviorTreeSUBB {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SwitchingSubbrain;
    void* GetSubbrainType() override;   // [27] sub_181A72480
    bool  _vf38() override;             // [38] sub_181A72470
};
static_assert(sizeof(C_SwitchingSubbrain) == 0x120, "C_SwitchingSubbrain must be 0x120 (operator new(288))");

}  // namespace wh::xgenaimodule
