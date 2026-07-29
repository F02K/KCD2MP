#pragma once
#include "C_VariableIndex.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LocalVariableIndex : C_VariableIndex -- the common
// per-agent/local variable index (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x28
// (create-site proven: operator new(40) in the lazy-getter sub_1805B66E8; inits
// exactly {vptr, m_parent=global, empty map}).  NO new data members.
// -----------------------------------------------
// RTTI TD rva 0x4FFD208; COL rva 0x40CCE28; vtable 0x183A3F770, 21 slots --
// identical to C_VariableIndex's except slot [0] (own deleting dtor
// sub_1805B6160).  Referenced from ~36 owner sites.

namespace wh::xgenaimodule {

class C_LocalVariableIndex : public C_VariableIndex {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LocalVariableIndex;
    ~C_LocalVariableIndex() override;   // [0] deleting dtor sub_1805B6160 (only differing slot)
};
static_assert(sizeof(C_LocalVariableIndex) == 0x28, "C_LocalVariableIndex must be 0x28 (operator new(40))");

}  // namespace wh::xgenaimodule
