#pragma once
#include "I_StoredDecision.h"

// -----------------------------------------------
// wh::xgenaimodule::C_VariableDecision : I_StoredDecision -- tiny stored-decision
// payload object (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10 (create-site proven
// x3: operator new(16) in sub_1832FDF64 / sub_18039FDFC / sub_1832FE1B8).
// -----------------------------------------------
// RTTI TD rva 0x4F65590; COL rva 0x40DD6B0; vtable 0x183A2D640, 8 slots.  Each
// factory writes { vptr, m_payload=0 }; one variant seeds m_payload via
// sub_18039FDA8(this, a2).  Slot bodies not individually decompiled [U roles] --
// the pure I_StoredDecision slots are implemented here.

namespace wh::xgenaimodule {

class C_VariableDecision : public I_StoredDecision {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_VariableDecision;
    void _vf1() override;   // [1] [U role]
    void _vf2() override;   // [2] [U role]
    void _vf3() override;   // [3] [U role]
    void _vf4() override;   // [4] [U role]
    void _vf5() override;   // [5] [U role]
    void _vf6() override;   // [6] [U role]
    void _vf7() override;   // [7] [U role]

    void* m_payload;   // +0x08  owned polymorphic obj = source->vtbl[3](source); deleting-dtor released on replace (seeder sub_18039FDA8); 0 for empty decisions [U class]
};
static_assert(sizeof(C_VariableDecision) == 0x10, "C_VariableDecision must be 0x10 (operator new(16))");

}  // namespace wh::xgenaimodule
